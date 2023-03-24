#pragma once
#include "Plot.hpp"
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Widgets/ChildWindow.hpp>
#include <TGUI/Widgets/VerticalLayout.hpp>
#include <TGUI/Widgets/ToggleButton.hpp>
#include <vector>

namespace graph {
class Graph {
public:
	Graph(tgui::Container::Ptr container, const sf::Font& font, const std::string& xAxisLabel = "");

	void addDataSet(
			const std::vector<float>& xAxis,
			const std::vector<float>& yAxis,
			const sf::Color& color,
			bool on = true
	);

	inline void update() {
		m_plot.scaleAxes();
		m_plot.generateVertices();
		draw();
	}

	inline void draw() {
		m_canvasPlot->clear(sf::Color{0xffffffff});
		m_canvasPlot->draw(m_plot);
		m_canvasPlot->display();
	}

	inline void toggleDataset(int index) {
		m_plot.toggleDatasetVisibility(index);
		draw();
	}
private:
	Plot m_plot;
	tgui::CanvasSFML::Ptr m_canvasPlot;

	tgui::ChildWindow::Ptr m_legend;
	tgui::VerticalLayout::Ptr m_layoutButtons;
};
}
