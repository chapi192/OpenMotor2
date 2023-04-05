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

	void updateLegendCanvas();

	void updateLegendWindow(float length);

	inline void update() {
		m_plot.scaleAxes();
		m_plot.generateVertices();
		updateLegendCanvas();
		draw();
	}

	inline void draw() {
		m_plotCanvas->clear(sf::Color{0xffffffff});
		m_plotCanvas->draw(m_plot);
		m_plotCanvas->display();
	}

	void toggleDataset(int index);
private:
	const sf::Color& getNextColor(int index);

	void legendMinimize();
	void legendMaximize();
private:
	Plot m_plot;
	float m_heightOffset;
	tgui::CanvasSFML::Ptr m_plotCanvas;

	tgui::ChildWindow::Ptr m_legendWindow;
	tgui::CanvasSFML::Ptr m_legendCanvas;
	std::vector<tgui::ToggleButton::Ptr> m_legendButtons;

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
