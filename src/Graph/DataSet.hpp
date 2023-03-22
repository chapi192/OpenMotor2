#pragma once
#include <vector>
#include <string>
#include <cassert>
#include "SFML/Graphics.hpp"

namespace Graph {
class DataSet {
public:
	enum Type {
		POINTS,
		LINE
	};
public:
	inline DataSet(
			const std::vector<float>& xValues = {},
			const std::vector<float>& yValues = {},
			const std::string& label = "Unlabeled",
			const sf::Color& color = sf::Color::Black,
			Type type = Type::LINE
	) :
		m_xValues(xValues),
		m_yValues(yValues),
		m_label(label),
		m_color(color),
		m_type(type)
	{
		assert(m_xValues.size() == m_yValues.size() && "DataSet: Incompatible data sizes.");
	}

	inline std::vector<float> getXValues() const {  return m_xValues;  }
	inline std::vector<float> getYValues() const {  return m_yValues;  }

	inline size_t getDataLength() const {  return m_xValues.size();  }

	/**
	 * Get a specific value
	 * @param  i index
	 * @return   value pair at i
	 */
	inline sf::Vector2f getDataValue(const size_t& i) const {
		return sf::Vector2f{m_xValues[i], m_yValues[i]};
	}

	/**
	 * Sets a specific value
	 * @param i         index
	 * @param valuePair new x and y values
	 */
	inline void setDataValue(const size_t& i, const sf::Vector2f& valuePair) {
		m_xValues[i] = valuePair.x;
		m_yValues[i] = valuePair.y;
	}

	inline sf::Color getColor() const            {  return m_color;  }
	inline void setColor(const sf::Color& color) {  m_color = color;  }

	inline std::string getLabel() const            {  return m_label;  }
	inline void setLabel(const std::string& label) {  m_label = label;  }

	inline Type getPlottingType() const    {  return m_type;  }
	inline void setPlottingType(Type type) {  m_type = type;  }

	/**
	 * Adding a pair of values to the dataset
	 * @param pair the pair to be added
	 */
	inline void pushPair(const sf::Vector2f& pair) {
		m_xValues.push_back(pair.x);
		m_yValues.push_back(pair.y);
	}

	/**
	 * Erases pairs of values to the dataset
	 * @param number of elements to erase
	 */
	inline void popFront(const size_t n = 1) {
		size_t erase_count = std::min(n, m_xValues.size());

		if (erase_count != 0) {
			m_xValues.erase(m_xValues.begin(), m_xValues.begin() + erase_count);
			m_yValues.erase(m_yValues.begin(), m_yValues.begin() + erase_count);
		}
	}
private:
	std::vector<float> m_yValues;
	std::vector<float> m_xValues;
	std::string m_label;
	sf::Color m_color;
	Type m_type;
};
}
