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
			bool on = true,
			const sf::Color& color = sf::Color::Transparent
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
		update();  // TODO: make this more modular rather than needing to generate vertices every time a dataset is toggled
	}
private:
	inline const sf::Color& getNextColor(int index) {
		int i = 0;
		while (i < inUseColors.size() && inUseColors[i])
			i++;
		if (i == inUseColors.size())
			return colors[i - 1];
		dataSetUsingColor[index] = i;
		inUseColors[i] = true;
		return colors[i];
	}

	void legendMinimize();
	void legendMaximize();
private:
	Plot m_plot;
	float m_heightOffset;
	tgui::CanvasSFML::Ptr m_canvasPlot;

	tgui::ChildWindow::Ptr m_legend;
	tgui::CanvasSFML::Ptr m_canvasLegend;
	std::vector<tgui::ToggleButton::Ptr> m_buttons;

	std::vector<sf::Color> colors {
		{  25, 122, 182 },
		{ 254, 123,   0 },
		{  55, 158,  43 },
		{ 212,  33,  24 },
		{ 145, 107, 190 },
		{ 139,  85,  73 },
		{ 225, 122, 193 },
		{ 190, 186,  17 },
		{  32, 191, 209 },
		{ 160, 160, 160 }
	};
	std::vector<bool> inUseColors;
	// -2 means predefined color
	// -1 means invisible
	std::vector<int> dataSetUsingColor;
};
}
