#pragma once
#include <vector>
#include <cmath>
#include <memory>

#include "SimData.hpp"
#include "Grains/Tube.hpp"
#include "Nozzle.hpp"
#include "Propellant.hpp"

namespace motor {
// R, in units of J / (kmol * K)
#define GAS_CONSTANT 8314.462618

#define BURNOUT_WEB_THRES      1 / 39370
#define BURNOUT_THRUST_PERCENT 0.1
#define TIMESTEP               0.03
#define AMB_PRESSURE           101325
class Motor {
public:
	Motor() {
		m_grains.push_back(std::make_unique<TubeGrain>(0.079375, 0.044831, PerforatedGrain::NEITHER, 0.015875));
		m_grains.push_back(std::make_unique<TubeGrain>(0.079375, 0.044831, PerforatedGrain::NEITHER, 0.015875));
		m_grains.push_back(std::make_unique<TubeGrain>(0.079375, 0.044831, PerforatedGrain::NEITHER, 0.015875));

		m_propellant.addProperties(Propellant::Properties{  103425.0,  806715.0, 1.9253259619746373e-06,   0.625, 1.1361, 1520, 39.9 });
		m_propellant.addProperties(Propellant::Properties{  806715.0, 1503110.0,     0.6656608561590813,  -0.313, 1.1361, 1520, 39.9 });
		m_propellant.addProperties(Propellant::Properties{ 1503110.0, 3792250.0,   0.009528121181782798, -0.0145, 1.1361, 1520, 39.9 });
		m_propellant.addProperties(Propellant::Properties{ 3792250.0, 7032900.0,  2.709667768835332e-06,  0.5245, 1.1361, 1520, 39.9 });
		m_propellant.addProperties(Propellant::Properties{ 7032900.0,10673460.0,    0.00417677261069904,   0.059, 1.1361, 1520, 39.9 });

		m_propellant.m_density = 1750;

		m_nozzle.m_convHAngle   = 65.0;
		m_nozzle.m_divHAngle    = 15.0;
		m_nozzle.m_efficiency   = 0.9;
		m_nozzle.m_diaExit      = 0.0254;
		m_nozzle.m_diaThroat    = 0.009144;
		m_nozzle.m_throatLength = 0.00127;
		m_nozzle.m_slagCoeff    = 0;
		m_nozzle.m_erosionCoeff = 0;
	}

	SimData burn() {
		const auto grainSize = m_grains.size();
		for (auto& grain : m_grains)
			grain->simulationPrep();

		unique_ptr<float> massSPtr    { new float[grainSize]{} };
		unique_ptr<float> massFlowSPtr{ new float[grainSize]{} };
		unique_ptr<float> massFluxSPtr{ new float[grainSize]{} };
		unique_ptr<float> regSPtr     { new float[grainSize]{} };
		unique_ptr<float> webSPtr     { new float[grainSize]{} };
		SimData sim{ grainSize,
				massSPtr,
				webSPtr,
				massFlowSPtr,
				massFluxSPtr,
				regSPtr
		};

		// Getting the raw pointer due to it having operator[]
		//   I hope this isn't undefined behavior
		auto mass     = massSPtr.get();
		auto massFlow = massFlowSPtr.get();
		auto massFlux = massFluxSPtr.get();
		auto reg      = regSPtr.get();
		auto web      = webSPtr.get();

		const auto dt = TIMESTEP;
		const auto& propDensity = m_propellant.m_density;
		const auto motorVolume = calcTotalVolume();

		{  // initialize the simulation data
			float kn = calcKN(reg, 0);
			float volumeLoading = 100 * (1 - (calcFreeVolume(reg) / motorVolume));
			for (int i = 0; i < grainSize; i++) {
				mass[i] = m_grains[i]->calcVolumeAtRegression(0) * propDensity;
				web[i] = m_grains[i]->calcWebLeft(0);
			}

			float pressure = calcIdealPressure(kn);
			sim.init(
					kn,
					pressure,
					volumeLoading
			);
			sim.pushGrainArrays();
		}

		float maxForce = -1;
		while (sim.m_force.back() > BURNOUT_THRUST_PERCENT / 100 * maxForce) {  // sim loop
			float currMassFlow = 0;  // current amount of mass flowing through the grain
			for (int i = 0; i < grainSize; i++) {
				const auto& grain = *m_grains[i];
				if (grain.calcWebLeft(reg[i]) > BURNOUT_WEB_THRES) {
					// Calculate change in regression at the current pressure
					float dReg = dt * m_propellant.calcBurnRate(sim.m_pressure.back());
					// Find the mass flux through the grain based on the mass flow fed into from grains above it
					massFlux[i] = grain.calcPeakMassFlux(currMassFlow, dt, reg[i], dReg, propDensity);
					// Find the mass of the grain after regression
					float newMass = grain.calcVolumeAtRegression(reg[i]) * propDensity;
					currMassFlow += (mass[i] - newMass) / dt;
					mass[i] = newMass;

					reg[i] += dReg;
					web[i] = grain.calcWebLeft(reg[i]);
				}
				massFlow[i] = currMassFlow;
			}
			sim.pushGrainArrays();

			sim.m_volumeLoading.push_back(100 * (1 - (calcFreeVolume(reg) / motorVolume)));

			float dThroat = sim.m_dThroat.back();
			float kn = calcKN(reg, dThroat);
			sim.m_kn.push_back(kn);

			float pressure = calcIdealPressure(kn);
			sim.m_pressure.push_back(pressure);

			float specificHeatRatio = m_propellant.getCombustionProperties(pressure).specificHeatRatio;
			float exitPressure = m_nozzle.calcExitPressure(specificHeatRatio, pressure);
			sim.m_exitPressure.push_back(exitPressure);

			float force = calcForce(pressure, dThroat, exitPressure);
			sim.m_force.push_back(force);

			sim.m_time.push_back(sim.m_time.back() + dt);

			float slagRate = 0;
			if (pressure != 0)
				slagRate = (1 / pressure) * m_nozzle.m_slagCoeff;
			float erosionRate = pressure * m_nozzle.m_erosionCoeff;
			float change = dt * (-2 * slagRate + 2 * erosionRate);
			sim.m_dThroat.push_back(dThroat + change);

			// TODO: Add more constraints to catch any errors
			if (maxForce < force)
				maxForce = force;
		}

		return sim;
	}

	float calcForce(float chamberPres, float dThroat, float exitPres) {
		float k = m_propellant.getCombustionProperties(chamberPres).specificHeatRatio;
		float thrustCoeff = m_nozzle.calcAdjustedThrustCoeff(chamberPres, AMB_PRESSURE, k, dThroat, exitPres);
		float thrust = thrustCoeff * m_nozzle.calcThroatArea(dThroat) * chamberPres;
		return std::max(thrust, 0.f);
	}

	float calcTotalVolume() {
		float sum = 0;
		for (auto& grain : m_grains)
			sum += grain->calcGrainBoundingVolume();
		return sum;
	}

	float calcFreeVolume(float reg[]) {
		float sum = 0;
		for (size_t i = 0; i != m_grains.size(); i++)
			sum += m_grains[i]->calcFreeVolume(reg[i]);
		return sum;
	}

	float calcBurningSurfaceArea(float reg[]) {
		float sum = 0;
		for (size_t i = 0; i != m_grains.size(); i++)
			if (m_grains[i]->calcWebLeft(reg[i]) > BURNOUT_WEB_THRES)
				sum += m_grains[i]->calcSurfaceAreaAtRegression(reg[i]);
		return sum;
	}

	inline float calcKN(float reg[], float dThroat) {
		float burningSurfaceArea = calcBurningSurfaceArea(reg);
		float nozzleArea = m_nozzle.calcThroatArea(dThroat);
		return burningSurfaceArea / nozzleArea;
	}

	float calcIdealPressure(float kn) {
		std::vector<std::pair<float, float>> propPressures;
		for (size_t i = 0; i != m_propellant.m_propArray.size(); i++) {
			auto& prop = m_propellant.m_propArray[i];
			float num = kn * m_propellant.m_density * prop.burnCoeff;
			float exponent = 1 / (1 - prop.burnExp);
			auto& k = prop.specificHeatRatio;
			float denom = std::sqrt((k / ((GAS_CONSTANT / prop.exhaustMolarMass) * prop.combustTemp)) * std::pow(2 / (k + 1), (k + 1) / (k - 1)));
			float propPressure = std::pow(num / denom, exponent);

			bool toReturn = i == 0 && propPressure < prop.maxPressure;  // is below the min pressure
			toReturn |= i == m_propellant.m_propArray.size() - 1 && propPressure > prop.minPressure;  // is above the max pressure
			toReturn |= prop.minPressure < propPressure && propPressure < prop.maxPressure;  // is inbetween bounds
			if (toReturn)
				return propPressure;
			propPressures.push_back({ std::min(std::abs(prop.minPressure - propPressure), std::abs(prop.maxPressure - propPressure)), propPressure });
		}

		std::pair<float, float> propPressure{ INFINITY, 0 };
		for (auto& prop : propPressures) {
			if (prop.first < propPressure.first)
				propPressure = prop;
		}

		return propPressure.second;
	}
private:
	Nozzle m_nozzle;
	Propellant m_propellant;
	std::vector<std::unique_ptr<Grain>> m_grains;
};
}
