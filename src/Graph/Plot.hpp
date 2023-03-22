#pragma once
#include "DataSet.hpp"

/**
 * Creates a string representation of a double with 'precision' number of digits after comma
 * @param  d         double to convert
 * @param  precision number of digits after comma
 * @return           String representation
 */
std::string toString(const double& d, const size_t& precision = 2);

// Loosely based on https://github.com/jerr-it/SFGraphing
namespace Graph {
class Plot : public sf::Drawable {
public:
	/**
	 * Initializes members
	 * @param position  top left position of plot
	 * @param size      width and height of plot
	 * @param margin    distance of axes from the borders of the plot
	 * @param font      sf::Font reference
	 * @param xLabel    label of x-axis
	 * @param axesColor color of axes, such as the lines and indicators
	 */
	Plot(
		const sf::Vector2f& position,
		const sf::Vector2f& size,
		float margin,
		const sf::Font& font,
		const std::string& xLabel = "",
		const sf::Color& axesColor = sf::Color::Black
	);

	/** Function for automatically deducing axes */
	void scaleAxes();

	/**
	 * Adds a dataset to the plot
	 * @param set the set to be added
	 */
	void addDataSet(const DataSet& data_set);

	/** Create SFML VertexArrays from Datasets */
	void generateVertices();

	/** Clear SFML VertexArrays */
	void clearVertices();

	inline void setAxesColor(const sf::Color& color) {
		m_axesColor = color;
	}

	inline void setPosition(const sf::Vector2f& position) {
		m_position = position;
	}
	inline void setSize(const sf::Vector2f& size) {
		m_size = size;
	}

	inline size_t getDataSetCount() {
		return m_dataSets.size();
	}
private:
	/** SFML Function override */
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	/**
	 * Converts plot coordinates to window coordinates
	 * @param  loc The position to convert
	 * @return     Converted Position
	 */
	sf::Vector2f coordToWindowPosition(const sf::Vector2f& loc);
private:
	sf::Vector2f m_position;
	sf::Vector2f m_size;

	float m_margin;

	std::vector<DataSet> m_dataSets;

	sf::Font m_font;

	std::string m_xAxisLabel;

	sf::Color m_axesColor;
	sf::Vector2f m_min;
	sf::Vector2f m_max;
	sf::Vector2f m_coordSteps;

	sf::VertexArray m_axesVertexArray;
	sf::VertexArray m_axesIndicatorVertexArray;

	std::vector<sf::VertexArray> m_dataSetsVertexArrays;
	std::vector<sf::Text> m_textElementArray;
};
}
