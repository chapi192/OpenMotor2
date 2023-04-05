#pragma once
#include "Grain.hpp"

#include "../Util.hpp"
using namespace geo;

namespace motor {
/** Cylinder that burns on one end. */
class EndBurningGrain : public Grain {
public:
	EndBurningGrain(float length, float diameter) :
			Grain( length, diameter ) {}

	virtual std::array<float, 2> calcEndPositions(float reg) const {
		return {0, m_length - reg};
	}

	virtual float calcWebLeft(float reg) const {
		return calcRegressedLength(reg);
	}

	virtual float calcSurfaceAreaAtRegression(float reg) const {
		return circleArea(m_diameter);
	}

	virtual float calcPortArea(float reg) const {
		return 0;
	}

	virtual float calcVolumeAtRegression(float reg) const {
		auto regLength = calcRegressedLength(reg);
		return cylinderVolume(m_diameter, regLength);
	}

	virtual float calcMassFlux(float massFlowIn, float dt, float reg, float dReg, float position, float density) const {
		return 0;
	}
};
}
