#include "Graph.hpp"
#include <TGUI/Widgets/ToggleButton.hpp>
using namespace graph;

Graph::Graph(tgui::Container::Ptr container, const sf::Font& font, const std::string& xAxisLabel) :
		m_plot{ {}, {}, 50, font, xAxisLabel },
		m_canvasPlot{ tgui::CanvasSFML::create() },
		m_legend{ tgui::ChildWindow::create("Legend", 0) },
		m_layoutButtons{ tgui::VerticalLayout::create() }
{
	container->add(m_canvasPlot);
	container->add(m_legend);
	m_legend->add(m_layoutButtons);

	m_plot.setSize(container->getSize());
	m_legend->setPosition(0.75 * container->getSize().x, 0.02 * container->getSize().y);
}

void Graph::addDataSet(
		const std::vector<float>& xAxis,
		const std::vector<float>& yAxis,
		const sf::Color& color,
		bool on
) {
	const int datasetID = m_plot.getDataSetCount();
	std::string str = "Dataset " + std::to_string(datasetID);
	DataSet set{ xAxis, yAxis, str, color, DataSet::LINE };
	m_plot.addDataSet(set);

	auto button = tgui::ToggleButton::create(str, on);
	if (!on)
		toggleDataset(datasetID);
	button->onToggle(&toggleDataset, this, datasetID);

	auto size = button->getSize();
	size.y += 10;
	m_layoutButtons->add(button);
	m_legend->setSize(size.x, m_layoutButtons->getWidgets().size() * size.y);
}