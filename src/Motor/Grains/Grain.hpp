#pragma once
#include <array>

#include "../Util.hpp"
using namespace geo;

namespace motor {
class Grain {
public:
	Grain(float length, float diameter) :
			m_length{ length }, m_diameter{ diameter } {}

	virtual void simulationPrep() {};

	virtual std::array<float, 2> calcEndPositions(float reg) const = 0;

	inline float calcRegressedLength(float reg) const {
		auto endPos = calcEndPositions(reg);
		return endPos[1] - endPos[0];
	}

	virtual float calcWebLeft(float reg) const = 0;

	virtual float calcSurfaceAreaAtRegression(float reg) const = 0;

	virtual float calcPortArea(float reg) const = 0;

	virtual float calcVolumeAtRegression(float reg) const = 0;

	inline float calcVolumeSlice(float reg, float dReg) const {
		return calcVolumeAtRegression(reg) - calcVolumeAtRegression(reg + dReg);
	}

	inline float calcGrainBoundingVolume() const {
		return cylinderVolume(m_diameter, m_length);
	}

	inline float calcFreeVolume(float reg) const {
		return calcGrainBoundingVolume() - calcVolumeAtRegression(reg);
	}

	virtual float calcMassFlux(float massIn, float dTime, float reg, float dReg, float position, float density) const = 0;

	inline float calcPeakMassFlux(float massIn, float dTime, float reg, float dReg, float density) const {
		return calcMassFlux(massIn, dTime, reg, dReg, calcEndPositions(reg)[1], density);
	}
protected:
	float m_length;
	float m_diameter;
};
}
