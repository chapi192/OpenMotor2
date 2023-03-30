#pragma once
#include <float.h>
#include <cmath>

#include "Util.hpp"
using namespace geo;

#include <iostream>

namespace motor {
class Nozzle {
public:
	inline float calcThroatArea(float dThroat=0) const {
		return circleArea(m_diaThroat + dThroat);
	}

	inline float calcExitArea() const {
		return circleArea(m_diaExit);
	}

	// TODO: calc exitPressure by solving for outPressure in the expansion ratio from pressure ratio function
	inline float calcExitPressure(float k, float inPressure) const {
		if (inPressure == 0)
			return 0;
		return 83383.8;  // currently what the outpressure would be with the constant inPressure from the endburning grain
	}

	inline float calcDivergenceLosses() const {
		float divAngleRad = radians(m_divHAngle);
		return (1 + std::cos(divAngleRad)) / 2;
	}

	inline float calcThroatLosses(float dThroat=0) const {
		// Returns the losses caused by the throat aspect ratio as described in this document:
		// http://rasaero.com/dloads/Departures%20from%20Ideal%20Performance.pdf
		float throatAspect = m_throatLength / (m_diaThroat + dThroat);
		if (throatAspect > 0.45)
				return 0.95;
		return 0.99 - (0.0333 * throatAspect);
	}

	inline float getSkinLosses() const {
		// Returns the losses due to drag on the nozzle surface as described here:
		// https://apps.dtic.mil/dtic/tr/fulltext/u2/a099791.pdf.
		// This is a constant for now, as the user likely can't measure this themselves.
		return 0.99;
	}

	float calcIdealThrustCoeff(float chamberPres, float ambPres, float specHeatR, float dThroat, float exitPres) const {
		if (chamberPres == 0)
			return 0;

		float term1 = 2 * std::pow(specHeatR, 2) / (specHeatR - 1);
		float term2 = std::pow(2 / (specHeatR + 1), (specHeatR + 1) / (specHeatR - 1));
		float term3 = 1 - std::pow(exitPres / chamberPres, (specHeatR - 1) / specHeatR);

		float exitArea = calcExitArea();
		float throatArea = calcThroatArea(dThroat);

		float momentumThrust = std::sqrt(term1 * term2 * term3);
		float pressureThrust = ((exitPres - ambPres) * exitArea) / (throatArea * chamberPres);

		return momentumThrust + pressureThrust;
	}

	float calcAdjustedThrustCoeff(float chamberPres, float ambPres, float specHeatR, float dThroat, float exitPres) const {
		float thrustCoeffIdeal = calcIdealThrustCoeff(chamberPres, ambPres, specHeatR, dThroat, exitPres);
		float divLoss          = calcDivergenceLosses();
		float throatLoss       = calcThroatLosses(dThroat);
		float skinLoss         = getSkinLosses();
		return divLoss * throatLoss * m_efficiency * (skinLoss * thrustCoeffIdeal + (1 - skinLoss));
	}
public:
	float m_diaThroat;
	float m_diaExit;
	float m_efficiency;
	float m_divHAngle;  // divergence half angle in degrees
	float m_convHAngle;  // convergence half angle in degrees
	float m_throatLength;
	float m_slagCoeff;
	float m_erosionCoeff;
};
}
