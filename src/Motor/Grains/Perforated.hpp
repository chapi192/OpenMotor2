#pragma once
#include "Grain.hpp"

namespace motor {
class PerforatedGrain : public Grain {
public:
	enum InhibitedEnds {
		NEITHER = 0,
		TOP     = 1 << 0,
		BOTTOM  = 1 << 1,
		BOTH    = TOP | BOTTOM
	};
public:
	PerforatedGrain(float length, float diameter, InhibitedEnds inhibitedEnds) :
			Grain( length, diameter ),
			m_inhibitedEnds{ inhibitedEnds } {}

	virtual std::array<float, 2> calcEndPositions(float reg) const {
		std::array<float, 2> endPos{ 0, m_length };
		if (!(m_inhibitedEnds & TOP))  // if top is not inhibited
			endPos[1] -= reg;
		if (!(m_inhibitedEnds & BOTTOM))  // if bottom is not inhibited
			endPos[0] += reg;
		return endPos;
	}

	virtual float calcCorePerimeter(float reg) const = 0;

	virtual float calcFaceArea(float reg) const = 0;

	float calcCoreSurfaceArea(float reg) const {
		float corePerimeter = calcCorePerimeter(reg);
		float coreArea = corePerimeter * calcRegressedLength(reg);
		return coreArea;
	}

	virtual float calcSurfaceAreaAtRegression(float reg) const {
		float faceArea = calcFaceArea(reg);
		float coreArea = calcCoreSurfaceArea(reg);

		int exposedFaces = !(m_inhibitedEnds & TOP) + !(m_inhibitedEnds & BOTTOM);
		return coreArea + (exposedFaces * faceArea);
	}

	virtual float calcVolumeAtRegression(float reg) const {
		return calcFaceArea(reg) * calcRegressedLength(reg);
	}

	virtual float calcWebLeft(float reg) const {
		float wallLeft = m_wallWeb - reg;
		if (m_inhibitedEnds == BOTH)
			return wallLeft;
		float lengthLeft = calcRegressedLength(reg);
		return std::min(lengthLeft, wallLeft);
	}

	virtual float calcPortArea(float reg) const {
		float faceArea = calcFaceArea(reg);
		float uncored = circleArea(m_diameter);
		return uncored - faceArea;
	}

	virtual float calcMassFlux(float massFlowIn, float dt, float reg, float dReg, float position, float density) const {
		auto endPos = calcEndPositions(reg);
		float massFlow = massFlowIn;

		if (position < endPos[0])  // Position above the top face
			return massFlow / circleArea(m_diameter);

		if (position <= endPos[1]) {  // Position in the grain
			// Mass flow includes the input, the mass from the top face, and mass from the tube up to the position.
			float top = 0;
			float countedCoreLength = position;
			if (!(m_inhibitedEnds & TOP)) {  // if top is not inhibited
				top = calcFaceArea(reg + dReg) * dReg * density;
				countedCoreLength -= endPos[0] + dReg;
			}
			float portAreaNew = calcPortArea(reg + dReg);
			float portAreaRegressed = portAreaNew - calcPortArea(reg);
			float massBurnedCore = portAreaRegressed * countedCoreLength * density;

			massFlow += (top + massBurnedCore) / dt;
			return massFlow / portAreaNew;
		}

		// Position past the grain end, mass flow includes the input and mass from the grain.
		massFlow += calcVolumeSlice(reg, dReg) * density / dt;
		return massFlow / circleArea(m_diameter);
	}
protected:
	InhibitedEnds m_inhibitedEnds;
	float m_wallWeb;
};
}
