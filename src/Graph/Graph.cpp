#include "Graph.hpp"
using namespace graph;
#include <cmath>

Graph::Graph(const tgui::Layout2d& size, const sf::Font& font, const std::string& xAxisLabel) :
		Container{ "Graph", true },
		m_plot{ {}, {}, 50, 1.05, font, xAxisLabel },
		m_plotCanvas{ tgui::CanvasSFML::create() },
		m_legendWindow{ tgui::ChildWindow::create("Legend", tgui::ChildWindow::Minimize) },
		m_legendCanvas{ tgui::CanvasSFML::create({ 2 * lineLength, "100%" }) }
{
	m_size = size;
	inUseColors.insert(inUseColors.begin(), colors.size(), false);

	add(m_plotCanvas);
	add(m_legendWindow);
	m_legendWindow->add(m_legendCanvas);

	m_plotCanvas->setSize("parent.size");
	m_plot.setSize(m_plotCanvas->getSize());
	m_legendWindow->setPosition("75%", "2%");
	m_legendWindow->setClientSize({0, 0});

	m_legendWindow->onMinimize(&legendMinimize, this);
	m_legendWindow->onMaximize(&legendMaximize, this);
}

void Graph::addDataSet(
		const std::vector<float>& xAxis,
		const std::vector<float>& yAxis,
		const std::string& label,
		bool on,
		const sf::Color& color
) {
	const int datasetID = m_plot.getDataSetCount();
	auto str = label == "" ? "Dataset " + std::to_string(datasetID) : label;
	auto col = color;
	if (color != sf::Color::Transparent)
		dataSetUsingColor.push_back(-2);
	else {
		dataSetUsingColor.push_back(-1);
		if (on)
			col = getNextColor(datasetID);
	}
	DataSet set{ xAxis, yAxis, str, col, DataSet::LINE };
	m_plot.addDataSet(set);

	auto button = tgui::ToggleButton::create(str, on);
	if (!on)
		toggleDataset(datasetID);
	button->onToggle(&toggleDataset, this, datasetID);

	tgui::Layout y = m_legendButtons.empty() ? 0 : bindBottom(m_legendButtons.back());
	button->setPosition(bindRight(m_legendCanvas), y - 1);
	button->setSize(button->getSize().x, (int)(button->getSize().y + 1));  // (int)y: height of button is initially a floating value so it would look ugly

	float length = button->getSize().x;
	m_legendWindow->add(button);
	m_legendButtons.push_back(button);

	updateLegendWindow(length);
}

void Graph::updateLegendCanvas() {
	m_legendCanvas->clear(sf::Color{0xf0f0f0ff});

	auto& dataSets = m_plot.getDataSets();
	auto& invisibility = m_plot.getDataSetInvisibility();

	auto position = tgui::Vector2f{ lineLength / 2.0f, (m_heightOffset - lineWidth) / 2.0f - 1};
	for (size_t i = 0; i != dataSets.size(); i++, position.y += m_heightOffset) {
		if (invisibility[i])
			continue;
		sf::RectangleShape rect;
		rect.setFillColor(dataSets[i].getColor());
		rect.setSize({ lineLength, lineWidth });
		rect.setPosition(position);
		m_legendCanvas->draw(rect);
	}

	m_legendCanvas->display();
}

void Graph::updateLegendWindow(float length) {
	float buttonLength = length;
	length += m_legendCanvas->getSize().x - 1;
	if (length < m_legendWindow->getClientSize().x)
		length = m_legendWindow->getClientSize().x;

	m_heightOffset = m_legendButtons[0]->getSize().y - 1;
	float height = m_heightOffset * m_legendButtons.size();
	m_legendWindow->setClientSize({length, height - 1});

	auto distToBottomRight = m_plotCanvas->getSize() - (m_legendWindow->getPosition() + m_legendWindow->getSize());
	distToBottomRight.x = distToBottomRight.x < 0 ? distToBottomRight.x : 0;
	distToBottomRight.y = distToBottomRight.y < 0 ? distToBottomRight.y : 0;
	auto distToTopLeft = m_legendWindow->getPosition() + distToBottomRight;
	distToTopLeft.x = distToTopLeft.x < 0 ? distToTopLeft.x : 0;
	distToTopLeft.y = distToTopLeft.y < 0 ? distToTopLeft.y : 0;
	m_legendWindow->setPosition(m_legendWindow->getPosition() + distToBottomRight - distToTopLeft);

	updateLegendCanvas();
}

void Graph::toggleDataset(int index) {
	m_plot.toggleDatasetVisibility(index);
	auto& idx = dataSetUsingColor[index];
	if (idx != -2) {
		if (!m_plot.getDataSetInvisibility()[index]) {
			m_plot.setDataSetColor(index, getNextColor(index));
		} else if (idx != -1) {
			if (idx < inUseColors.size())
				inUseColors[idx] = false;
			idx = -1;
		}
	}
	m_plot.setAxes();
	update();  // TODO: make this more modular rather than needing to generate vertices every time a dataset is toggled
}

const sf::Color& Graph::getNextColor(int index) {
	int i = 0;
	while (i < inUseColors.size() && inUseColors[i])
		i++;
	if (i == inUseColors.size())
		return colors[i - 1];
	dataSetUsingColor[index] = i;
	inUseColors[i] = true;
	return colors[i];
}

void Graph::legendMinimize() {
	m_legendWindow->setHeight(m_legendWindow->getSize().y - m_legendWindow->getClientSize().y);
	m_legendWindow->setTitleButtons(tgui::ChildWindow::Maximize);
}
void Graph::legendMaximize() {
	float length = m_legendWindow->getClientSize().x;
	length -= m_legendCanvas->getSize().x - 1;  // adjusts for the respective `+=` in updateLegend
	updateLegendWindow(length);
	m_legendWindow->setTitleButtons(tgui::ChildWindow::Minimize);
}

bool Graph::mouseWheelScrolled(float delta, tgui::Vector2f pos) {
	float zoomAmount = std::pow(1.05, delta);
	auto origin = m_plot.windowPositionToCoord(pos - m_plotCanvas->getPosition());

	zoomPlot(sf::Vector2f{1,1} * zoomAmount, origin);
	update();
	return Widget::mouseWheelScrolled(delta, pos);
}

void Graph::zoomPlot(sf::Vector2f zoom, sf::Vector2f origin) {
	m_plot.scaleAxes(zoom, origin);
}
