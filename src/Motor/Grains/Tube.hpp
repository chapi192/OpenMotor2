#pragma once
#include "PerforatedGrain.hpp"
#include <stdexcept>

namespace motor {
class TubeGrain : public PerforatedGrain {
public:
	TubeGrain(float length, float diameter, InhibitedEnds inhibitedEnds, float coreDiameter) :
		PerforatedGrain( length, diameter, inhibitedEnds ),
		m_coreDia{ coreDiameter }
	{
		if (coreDiameter > diameter) {
			std::string msg = "coreDiameter > diameter: ";
			msg += coreDiameter;
			msg += " > ";
			msg += diameter;
			throw std::invalid_argument{ msg };
		}
	}

	virtual void simulationPrep() {
		m_wallWeb = (m_diameter - m_coreDia) / 2;
	}

	virtual float calcCorePerimeter(float reg) const {
		return circlePerimeter(m_coreDia + 2 * reg);
	}

	virtual float calcFaceArea(float reg) const {
		float outer = circleArea(m_diameter);
		float inner = circleArea(m_coreDia + 2 * reg);
		return outer - inner;
	}
private:
	float m_coreDia;
};
}
