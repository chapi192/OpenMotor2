#include "Graph.hpp"
using namespace graph;

Graph::Graph(tgui::Container::Ptr container, const sf::Font& font, const std::string& xAxisLabel) :
		m_plot{ {}, {}, 50, 1.05, font, xAxisLabel },
		m_canvasPlot{ tgui::CanvasSFML::create() },
		m_legend{ tgui::ChildWindow::create("Legend", 0) },
		m_canvasLegend{ tgui::CanvasSFML::create({ 2 * lineLength, "100%" }) }
{
	inUseColors.insert(inUseColors.begin(), colors.size(), false);

	container->add(m_canvasPlot);
	container->add(m_legend);
	m_legend->add(m_canvasLegend);

	m_plot.setSize(container->getSize());
	m_legend->setPosition(0.75 * container->getSize().x, 0.02 * container->getSize().y);
	m_legend->setClientSize({0, 0});
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

	tgui::Layout y = m_buttons.empty() ? 0 : bindBottom(m_buttons.back());
	button->setPosition(bindRight(m_canvasLegend), y - 1);
	button->setSize(button->getSize().x, (int)(button->getSize().y + 1));  // (int)y: height of button is initially a floating value so it would look ugly

	float length = button->getSize().x;
	m_legend->add(button);
	m_buttons.push_back(button);

	updateLegend(length);
}

void Graph::updateCanvasLegend() {
	m_canvasLegend->clear(sf::Color{0xf0f0f0ff});

	auto& dataSets = m_plot.getDataSets();
	auto& invisibility = m_plot.getDataSetInvisibility();

	auto position = tgui::Vector2f{ 0.5f * lineLength, 0.5f * m_heightOffset - lineWidth / 2};
	for (size_t i = 0; i != dataSets.size(); i++, position.y += m_heightOffset) {
		if (invisibility[i])
			continue;
		sf::RectangleShape rect;
		rect.setFillColor(dataSets[i].getColor());
		rect.setSize({ lineLength, lineWidth });
		rect.setPosition(position);
		m_canvasLegend->draw(rect);
	}

	m_canvasLegend->display();
}

void Graph::updateLegend(float length) {
	float buttonLength = length;
	length += m_canvasLegend->getSize().x;
	if (length < m_legend->getClientSize().x)
		length = m_legend->getClientSize().x;

	m_heightOffset = m_buttons[0]->getSize().y - 1;
	float height = m_heightOffset * m_buttons.size();
	m_legend->setClientSize({length - 1, height - 1});

	auto distanceToBottomRight = m_canvasPlot->getSize() - (m_legend->getPosition() + m_legend->getSize());
	distanceToBottomRight.x = distanceToBottomRight.x < 0 ? distanceToBottomRight.x : 0;
	distanceToBottomRight.y = distanceToBottomRight.y < 0 ? distanceToBottomRight.y : 0;
	m_legend->setPosition(m_legend->getPosition() + distanceToBottomRight);

	updateCanvasLegend();
}
