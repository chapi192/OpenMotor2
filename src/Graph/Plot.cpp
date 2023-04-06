#include "Plot.hpp"
#include <cmath>
#include <sstream>
#include <array>
#include <float.h>
using namespace graph;

std::string toString(float d, size_t precision) {
	if (std::abs(d) < FLT_EPSILON)
		return "0";
	std::ostringstream stream;
	if (std::abs(d) < 1)
		precision -= 1;  // to account for the 0
	stream.precision(precision);
	stream << d;
	return stream.str();
}

std::string toStringSci(float d, size_t precision) {
	std::ostringstream stream;
	stream.precision(precision - 1);  // to account for the first digit
	stream.setf(std::ios::scientific);
	stream << d;
	return stream.str();
}

int scinotScalingDegree(float min, float max, size_t precision = 3) {
	auto textMin = toStringSci(min, precision);
	auto textMax = toStringSci(max, precision);
	// grab the degree from the string
	int degreeMin = std::stoi(textMin.substr(textMin.find('e') + 1));
	int degreeMax = std::stoi(textMax.substr(textMax.find('e') + 1));

	if ((degreeMin == 0 && degreeMax == 0))
		return 0;

	auto degree = degreeMax;
	if (std::abs(min) > FLT_EPSILON && degreeMin > degreeMax)
		degree = degreeMin;

	degree -= 1;
	if (std::abs(degree) <= precision / 2)
		return 0;
	return degree;
}

Plot::Plot(
	const sf::Vector2f& position,
	const sf::Vector2f& size,
	float margin,
	float padding,
	const sf::Font& font,
	const std::string& xLabel,
	const sf::Color& axesColor
) :
		m_position(position),
		m_size(size),
		m_margin(margin),
		m_padding(padding),
		m_font(font),
		m_xAxisLabel(xLabel),
		m_axesColor(axesColor) {}

std::array<float, 3> Plot::calcAutoscale(float min, float max) {
	float range = max - min;

	float logRange = std::log10(range);
	float adjLogRange = std::floor(logRange) - 1;
	float interval = std::pow(10, adjLogRange);

	float diff = logRange - std::floor(logRange);
	if (diff > 0.12)
		interval *= 2;
	if (diff > 0.4)
		interval *= 2.5;
	if (diff > 0.75)
		interval *= 2;

	float minAdj = interval * std::ceil (min / interval);
	float maxAdj = interval * std::floor(max / interval);

	return { interval, minAdj, maxAdj };
}

void Plot::setAxes() {
	float xMin = INFINITY;
	float yMin = INFINITY;
	float xMax = -INFINITY;
	float yMax = -INFINITY;

	for (size_t i = 0; i < m_dataSets.size(); i++) {
		if (m_dataSetInvisible[i])
			continue;
		for (auto& val : m_dataSets[i].getXValues()) {
			xMin = (xMin < val) ? xMin : val;
			xMax = (xMax > val) ? xMax : val;
		}
		for (auto& val : m_dataSets[i].getYValues()) {
			yMin = (yMin < val) ? yMin : val;
			yMax = (yMax > val) ? yMax : val;
		}
	}

	m_min.x = xMin;
	m_min.y = yMin;
	m_max.x = xMax;
	m_max.y = yMax;

	if (yMin == INFINITY) {
		m_min.x = 0;
		m_max.x = 1;
	}
	if (yMin == yMax || yMin == INFINITY) {
		m_min.y = -1;
		m_max.y = 1;
	}

	scaleAxes({1, 1}, {(m_max.x + m_min.x) / 2, (m_max.y + m_min.y) / 2});
}

void Plot::scaleAxes(const sf::Vector2f& zoom, const sf::Vector2f& origin) {
	m_min.x = origin.x + (m_min.x - origin.x) / zoom.x;
	m_min.y = origin.y + (m_min.y - origin.y) / zoom.y;
	m_max.x = origin.x + (m_max.x - origin.x) / zoom.x;
	m_max.y = origin.y + (m_max.y - origin.y) / zoom.y;

	auto resY = calcAutoscale(m_min.y, m_max.y);
	m_coordSteps.y = resY[0];
	m_adjMin.y = resY[1];
	m_adjMax.y = resY[2];

	auto resX = calcAutoscale(m_min.x, m_max.x);
	m_coordSteps.x = resX[0];
	m_adjMin.x = resX[1];
	m_adjMax.x = resX[2];

	// adjust coordSteps when very small so that they can actually increment adjMin to reach adjMax
	while (m_adjMax.x + m_coordSteps.x == m_adjMax.x || m_adjMin.x + m_coordSteps.x == m_adjMin.x)
		m_coordSteps.x *= 1.01;
	while (m_adjMax.y + m_coordSteps.y == m_adjMax.y || m_adjMin.y + m_coordSteps.y == m_adjMin.y)
		m_coordSteps.y *= 1.01;
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
	const int precision = 3;
	float tickLength = m_margin / 10;

	const float xLimit = m_adjMax.x + m_coordSteps.x / 2 * m_padding;
	sf::Vector2f windowPosition = coordToWindowPosition({ 0, m_min.y });
	for (float x = m_adjMin.x; x < xLimit; x += m_coordSteps.x) {
		windowPosition.x = coordToWindowPosition_padded({ x, 0 }).x;

		m_axesIndicatorVertexArray.append({ windowPosition, m_axesColor });
		m_axesIndicatorVertexArray.append({ { windowPosition.x, windowPosition.y + tickLength }, m_axesColor });

		sf::Text indicatorText{ initText };
		indicatorText.setString(toString(x, precision));
		sf::FloatRect size = indicatorText.getLocalBounds();
		indicatorText.setOrigin(size.left + size.width / 2, size.top);
		indicatorText.setPosition(windowPosition.x, windowPosition.y + 2 * tickLength);

		m_textElementArray.push_back(indicatorText);
	}

	// get scaling for the y-axis
	int degree = scinotScalingDegree(m_adjMin.y, m_adjMax.y, precision);
	float scaling = std::pow(0.1, degree);
	const float yLimit = m_adjMax.y + m_coordSteps.y / 2 * m_padding;
	windowPosition = coordToWindowPosition({ m_min.x, 0 });
	for (float y = m_adjMin.y; y < yLimit; y += m_coordSteps.y) {
		windowPosition.y = coordToWindowPosition_padded({ 0, y }).y;

		m_axesIndicatorVertexArray.append({ windowPosition, m_axesColor });
		m_axesIndicatorVertexArray.append({ { windowPosition.x - tickLength, windowPosition.y }, m_axesColor });

		sf::Text indicatorText{ initText };
		indicatorText.setString(toString(y * scaling, precision));
		sf::FloatRect size = indicatorText.getLocalBounds();
		indicatorText.setOrigin(size.left + size.width, size.top + size.height / 2);
		indicatorText.setPosition(windowPosition.x - 2 * tickLength, windowPosition.y);

		m_textElementArray.push_back(indicatorText);
	}
	/* y-axis sci-notation label */
	if (degree != 0) {
		std::string scinotStr = "e";
		if (degree > 0)
			scinotStr += '+';
		scinotStr += std::to_string(degree);

		sf::Text scinotLabel{ initText };
		scinotLabel.setString(scinotStr);
		scinotLabel.setPosition(coordToWindowPosition({ m_min.x, m_max.y }) + sf::Vector2f{0, -0.4f * m_margin});
		sf::FloatRect yaxisSize = scinotLabel.getLocalBounds();
		scinotLabel.setOrigin(yaxisSize.left + yaxisSize.width, yaxisSize.top + yaxisSize.height);
		m_textElementArray.push_back(scinotLabel);
	}

	/* Generate graphs */
	for (const auto& dataset : m_dataSets) {
		sf::VertexArray graph;
		DataSet::Type type = dataset.getPlottingType();

		sf::Color col = dataset.getColor();

		if (type == DataSet::POINTS) {
			graph.setPrimitiveType(sf::PrimitiveType::Quads);

			for (size_t i = 0; i < dataset.getDataLength(); i++) {
				sf::Vector2f windowPosition = coordToWindowPosition_padded(dataset.getDataValue(i));

				graph.append({ { windowPosition.x - 2, windowPosition.y - 2 }, col});
				graph.append({ { windowPosition.x + 2, windowPosition.y - 2 }, col});
				graph.append({ { windowPosition.x + 2, windowPosition.y + 2 }, col});
				graph.append({ { windowPosition.x - 2, windowPosition.y + 2 }, col});
			}
		} else if (type == DataSet::LINE) {
			graph.setPrimitiveType(sf::PrimitiveType::LinesStrip);

			for (size_t i = 0; i < dataset.getDataLength(); i++) {
				sf::Vector2f windowPosition = coordToWindowPosition_padded(dataset.getDataValue(i));

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

sf::Vector2f Plot::windowPositionToCoord_padded(const sf::Vector2f& pos) {
	auto axisLength = getAxesSize();

	auto range = m_max - m_min;

	float localPosX = pos.x - (m_position.x + m_margin);
	float localPosY = pos.y - (m_position.y - m_margin);

	localPosY = m_size.y - localPosY;
	localPosX *= m_padding / axisLength.x;
	localPosY *= m_padding / axisLength.y;
	localPosX -= (m_padding - 1) / 2;
	localPosY -= (m_padding - 1) / 2;
	localPosX = (localPosX * range.x) + m_min.x;
	localPosY = (localPosY * range.y) + m_min.y;

	return { localPosX, localPosY };
}

sf::Vector2f Plot::coordToWindowPosition(const sf::Vector2f& coords) {
	auto axisLength = getAxesSize();

	auto range = m_max - m_min;
	auto distMin = coords - m_min;

	sf::Vector2f localPosition;
	localPosition.x = axisLength.x * distMin.x / range.x;
	localPosition.y = axisLength.y * distMin.y / range.y;
	localPosition.y = m_size.y - localPosition.y;  // adjust y to be from bottom to top

	sf::Vector2f windowPosition{ localPosition };
	windowPosition.x += m_position.x + m_margin;
	windowPosition.y += m_position.y - m_margin;
	return windowPosition;
}

sf::Vector2f Plot::coordToWindowPosition_padded(const sf::Vector2f& coords) {
	auto axisLength = getAxesSize();

	auto range = m_max - m_min;
	auto distMin = coords - m_min;

	sf::Vector2f localPosition;
	localPosition.x = axisLength.x / m_padding * (distMin.x / range.x + (m_padding - 1) / 2);
	localPosition.y = axisLength.y / m_padding * (distMin.y / range.y + (m_padding - 1) / 2);
	localPosition.y = m_size.y - localPosition.y;  // adjust y to be from bottom to top

	sf::Vector2f windowPosition{ localPosition };
	windowPosition.x += m_position.x + m_margin;
	windowPosition.y += m_position.y - m_margin;
	return windowPosition;
}
