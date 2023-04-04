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
		Properties(
				float minPressure, float maxPressure,
				float burnCoeff, float burnExp,
				float specificHeatRatio,
				float combustTemp,
				float exhaustMolarMass
		) :
				minPressure{ minPressure }, maxPressure{ maxPressure },
				burnCoeff{ burnCoeff }, burnExp{ burnExp },
				specificHeatRatio{ specificHeatRatio },
				combustTemp      { combustTemp },
				exhaustMolarMass { exhaustMolarMass } {}
	};
public:
	/** Keeps the array sorted from minPressure to maxPressure. */
	inline void addProperties(const Properties& prop) {
		auto iter{ m_propArray.begin() };
		while (iter != m_propArray.end()) {
			if (iter->minPressure > prop.minPressure)
				break;
			iter++;
		}
		m_propArray.insert(iter, prop);
	}

	inline const Properties& getCombustionProperties(float pressure) const {
		if (pressure < m_propArray[0].minPressure)
			return m_propArray[0];

		int i = 0;
		while (i != m_propArray.size() && pressure > m_propArray[i].maxPressure)
			i++;

		if (i == m_propArray.size())
			return m_propArray.back();
		if (pressure > m_propArray[i].minPressure)
			return m_propArray[i];
		if (m_propArray[i].minPressure - pressure < pressure - m_propArray[i - 1].maxPressure)
			return m_propArray[i];
		return m_propArray[i - 1];
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
