#pragma once
#include "Plot.hpp"
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Widgets/ChildWindow.hpp>
#include <TGUI/Widgets/HorizontalLayout.hpp>
#include <TGUI/Widgets/VerticalLayout.hpp>
#include <TGUI/Widgets/ToggleButton.hpp>
#include <vector>

namespace graph {
class Graph {
private:
	static const int lineLength = 10;
	static const int lineWidth = 3;
public:
	Graph(tgui::Container::Ptr container, const sf::Font& font, const std::string& xAxisLabel = "");

	void addDataSet(
			const std::vector<float>& xAxis,
			const std::vector<float>& yAxis,
			const std::string& label = "",
			const sf::Color& color = sf::Color::Black,
			bool on = true
	);
	void updateCanvasLegend();

	void updateLegend(float length);

	inline void update() {
		m_plot.scaleAxes();
		m_plot.generateVertices();
		updateCanvasLegend();
		draw();
	}

	inline void draw() {
		m_canvasPlot->clear(sf::Color{0xffffffff});
		m_canvasPlot->draw(m_plot);
		m_canvasPlot->display();
	}

	inline void toggleDataset(int index) {
		m_plot.toggleDatasetVisibility(index);
		update();  // TODO: make this more modular rather than needing to generate vertices every time a dataset is toggled
	}
private:
	Plot m_plot;
	float m_heightOffset;
	tgui::CanvasSFML::Ptr m_canvasPlot;

	tgui::ChildWindow::Ptr m_legend;
	tgui::CanvasSFML::Ptr m_canvasLegend;
	std::vector<tgui::ToggleButton::Ptr> m_buttons;
};
}
