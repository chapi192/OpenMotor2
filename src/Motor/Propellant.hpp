#pragma once
#include <vector>
#include <cmath>

namespace motor {
class Propellant {
public:
	struct Properties {
		float minPressure;
		float maxPressure;
		float burnCoeff;
		float burnExp;
		float specificHeatRatio;
		float combustTemp;
		float exhaustMolarMass;
	};
public:
	// TODO: Should keep the array sorted from minPressure to maxPressure.
	inline void addProperties(const Properties& prop) {
		m_propArray.push_back(prop);
	}

	// TODO: Account for multiple properties in the array
	inline const Properties& getCombustionProperties(float pressure) const {
		return m_propArray[0];
	}

	inline float calcBurnRate(float pressure) const {
		auto& prop = getCombustionProperties(pressure);
		return prop.burnCoeff * std::pow(pressure, prop.burnExp);
	}
public:
	// Array of properties of the propellant when combusting in specified pressure ranges.
	std::vector<Properties> m_propArray;
	float m_density;
};
}
