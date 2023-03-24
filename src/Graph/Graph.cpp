#include "Graph.hpp"
using namespace graph;

Graph::Graph(tgui::Container::Ptr container, const sf::Font& font, const std::string& xAxisLabel) :
		m_plot{ {}, {}, 50, font, xAxisLabel },
		m_canvasPlot{ tgui::CanvasSFML::create() },
		m_legend{ tgui::ChildWindow::create("Legend", 0) },
		m_canvasLegend{ tgui::CanvasSFML::create({ 2 * lineLength, "100%" }) }
{
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
		const sf::Color& color,
		bool on
) {
	const int datasetID = m_plot.getDataSetCount();
	auto str = label == "" ? "Dataset " + std::to_string(datasetID) : label;
	DataSet set{ xAxis, yAxis, str, color, DataSet::LINE };
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

void Graph::updateLegend(float length) {
	float buttonLength = length;
	length += m_canvasLegend->getSize().x;
	if (length < m_legend->getClientSize().x)
		length = m_legend->getClientSize().x;

	float heightOffset = m_buttons[0]->getSize().y - 1;
	float height = heightOffset * m_buttons.size();
	m_legend->setClientSize({length - 1, height - 1});

	m_canvasLegend->clear(sf::Color{0xf0f0f0ff});

	auto& dataSets = m_plot.getDataSets();
	auto position = tgui::Vector2f{ 0.5f * lineLength, 0.5f * heightOffset};
	sf::VertexArray arr{ sf::PrimitiveType::Lines, 2 * dataSets.size() };
	for (auto& dataSet : dataSets) {
		arr.append({position, dataSet.getColor()});
		position.x += lineLength;
		arr.append({position, dataSet.getColor()});
		position.x -= lineLength;
		position.y += heightOffset;
	}
	m_canvasLegend->draw(arr);

	m_canvasLegend->display();
}
