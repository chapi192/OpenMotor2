#include "Plot.hpp"
#include <cmath>
#include <sstream>
using namespace Graph;

std::string toString(const double& d, const size_t& precision) {
	std::stringstream stream;
	stream.precision(precision);
	stream << std::fixed << d;
	return stream.str();
}

void Plot::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(m_axesVertexArray, states);
	target.draw(m_axesIndicatorVertexArray, states);
	for (size_t i = 0; i < m_dataSetsVertexArrays.size(); i++) {
		if (m_dataSetInvisible[i])
			continue;
		target.draw(m_dataSetsVertexArrays[i], states);
	}
	for (size_t i = 0; i < m_textElementArray.size(); i++) {
		target.draw(m_textElementArray[i], states);
	}
}

sf::Vector2f Plot::coordToWindowPosition(const sf::Vector2f& coords) {
	sf::Vector2f windowPosition;

	float xAxisLength = m_size.x - 2 * m_margin;
	float yAxisLength = m_size.y - 2 * m_margin;

	windowPosition.x = m_position.x + (           m_margin + xAxisLength * ((coords.x - m_min.x) / (m_max.x - m_min.x) ));
	windowPosition.y = m_position.y + (m_size.y - m_margin - yAxisLength * ((coords.y - m_min.y) / (m_max.y - m_min.y) ));

	return windowPosition;
}

Plot::Plot(
	const sf::Vector2f& position,
	const sf::Vector2f& size,
	float margin,
	const sf::Font& font,
	const std::string& xLabel,
	const sf::Color& axesColor
) :
		m_position(position),
		m_size(size),
		m_margin(margin),
		m_font(font),
		m_xAxisLabel(xLabel),
		m_axesColor(axesColor) {}

void Plot::scaleAxes() {
	float xMin = INFINITY;
	float yMin = INFINITY;
	float xMax = -INFINITY;
	float yMax = -INFINITY;

	for (const auto& dataset : m_dataSets) {
		for (auto& val : dataset.getXValues()) {
			xMin = (xMin < val) ? xMin : val;
			xMax = (xMax > val) ? xMax : val;
		}
		for (auto& val : dataset.getYValues()) {
			yMin = (yMin < val) ? yMin : val;
			yMax = (yMax > val) ? yMax : val;
		}
	}

	m_min.x = xMin;
	m_min.y = yMin;
	m_max.x = xMax;
	m_max.y = yMax;

	const int stepCount = 8;
	m_coordSteps.x = (xMax - xMin) / stepCount;
	m_coordSteps.y = (yMax - yMin) / stepCount;
}

void Plot::addDataSet(const DataSet& data_set) {
	m_dataSets.push_back(data_set);
	m_dataSetInvisible.push_back(false);
}

void Plot::generateVertices() {
	this->clearVertices();
	const auto characterSize = static_cast<unsigned int>(m_margin * 0.3);
	sf::Text initText;
	initText.setFont(m_font);
	initText.setCharacterSize(characterSize);
	initText.setFillColor(m_axesColor);

	/* Axes line */
	m_axesVertexArray.setPrimitiveType(sf::LineStrip);
	int topLeftIdx = m_axesVertexArray.getVertexCount();
	m_axesVertexArray.append(sf::Vertex{coordToWindowPosition({ m_min.x, m_max.y }), m_axesColor});  // top left
	m_axesVertexArray.append(sf::Vertex{coordToWindowPosition(  m_min             ), m_axesColor});  // origin
	m_axesVertexArray.append(sf::Vertex{coordToWindowPosition({ m_max.x, m_min.y }), m_axesColor});  // bottom right
	m_axesVertexArray.append(sf::Vertex{coordToWindowPosition(  m_max             ), m_axesColor});  // top right
	m_axesVertexArray.append(m_axesVertexArray[topLeftIdx]);  // top left

	/* Axes labels */
	sf::Text xaxislabel{ initText };
	xaxislabel.setString(m_xAxisLabel);
	xaxislabel.setPosition(coordToWindowPosition({ (m_max.x + m_min.x) / 2, m_min.y }) + sf::Vector2f{0, 0.6f * m_margin});
	sf::FloatRect tSize = xaxislabel.getLocalBounds();
	xaxislabel.setOrigin(tSize.left + tSize.width / 2, tSize.top);
	m_textElementArray.push_back(xaxislabel);

	/* Axes indicators */
	m_axesIndicatorVertexArray.setPrimitiveType(sf::PrimitiveType::Lines);
	float tickLength = m_margin / 10;
	const float xLimit = m_max.x + m_coordSteps.x / 2;
	for (float x = m_min.x; x < xLimit; x += m_coordSteps.x) {
		sf::Vector2f windowPosition = coordToWindowPosition({ x, m_min.y });

		m_axesIndicatorVertexArray.append({ windowPosition, m_axesColor });
		m_axesIndicatorVertexArray.append({ { windowPosition.x, windowPosition.y + tickLength }, m_axesColor });

		sf::Text indicatorText{ initText };
		indicatorText.setString(toString(x));
		sf::FloatRect size = indicatorText.getLocalBounds();
		indicatorText.setOrigin(size.left + size.width / 2, size.top);
		indicatorText.setPosition(windowPosition.x, windowPosition.y + 2 * tickLength);

		m_textElementArray.push_back(indicatorText);
	}
	const float yLimit = m_max.y + m_coordSteps.y / 2;
	for (float y = m_min.y; y < yLimit; y += m_coordSteps.y) {
		sf::Vector2f windowPosition = coordToWindowPosition({ m_min.x, y });

		m_axesIndicatorVertexArray.append({ windowPosition, m_axesColor });
		m_axesIndicatorVertexArray.append({ { windowPosition.x - tickLength, windowPosition.y }, m_axesColor });

		sf::Text indicatorText{ initText };
		indicatorText.setString(toString(y));
		sf::FloatRect size = indicatorText.getLocalBounds();
		indicatorText.setOrigin(size.left + size.width, size.top + size.height / 2);
		indicatorText.setPosition(windowPosition.x - 2 * tickLength, windowPosition.y);

		m_textElementArray.push_back(indicatorText);
	}

	/* Generate graphs */
	for (const auto& dataset : m_dataSets) {
		sf::VertexArray graph;
		DataSet::Type type = dataset.getPlottingType();

		sf::Color col = dataset.getColor();

		if (type == DataSet::POINTS) {
			graph.setPrimitiveType(sf::PrimitiveType::Quads);

			for (size_t i = 0; i < dataset.getDataLength(); i++) {
				sf::Vector2f windowPosition = coordToWindowPosition(dataset.getDataValue(i));

				graph.append({ { windowPosition.x - 2, windowPosition.y - 2 }, col});
				graph.append({ { windowPosition.x + 2, windowPosition.y - 2 }, col});
				graph.append({ { windowPosition.x + 2, windowPosition.y + 2 }, col});
				graph.append({ { windowPosition.x - 2, windowPosition.y + 2 }, col});
			}
		} else if (type == DataSet::LINE) {
			graph.setPrimitiveType(sf::PrimitiveType::LinesStrip);

			for (size_t i = 0; i < dataset.getDataLength(); i++) {
				sf::Vector2f windowPosition = coordToWindowPosition(dataset.getDataValue(i));

				graph.append(sf::Vertex(windowPosition, col));
			}
		}

		m_dataSetsVertexArrays.push_back(graph);
	}
}

void Plot::toggleDatasetVisibility(int index) {
	m_dataSetInvisible[index] = !m_dataSetInvisible[index];
}

void Plot::clearVertices() {
	m_axesVertexArray.clear();
	m_axesIndicatorVertexArray.clear();
	m_dataSetsVertexArrays.clear();
	m_textElementArray.clear();
}
