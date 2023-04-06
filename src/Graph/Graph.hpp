#pragma once
#include "Plot.hpp"
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Widgets/ChildWindow.hpp>
#include <TGUI/Widgets/HorizontalLayout.hpp>
#include <TGUI/Widgets/VerticalLayout.hpp>
#include <TGUI/Widgets/ToggleButton.hpp>
#include <vector>

namespace graph {
class Graph : public tgui::Container {
private:
	static const int lineLength = 10;
	static const int lineWidth = 3;
public:
	typedef std::shared_ptr<Graph> Ptr;
	typedef std::shared_ptr<const Graph> ConstPtr;

	Graph(const tgui::Layout2d& size, const sf::Font& font, const std::string& xAxisLabel = "");

	static Ptr create(const tgui::Layout2d& size, const sf::Font& font, const std::string& xAxisLabel = "") {
		return std::make_shared<Graph>(size, font, xAxisLabel);
	}

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

	void zoomPlot(sf::Vector2f zoom, sf::Vector2f origin);
private:
	bool mouseWheelScrolled(float delta, tgui::Vector2f pos) override;

	bool isMouseOnWidget(tgui::Vector2f pos) const override {
		return tgui::FloatRect{ getPosition().x, getPosition().y, getSize().x, getSize().y }.contains(pos);
	}

	void draw(tgui::BackendRenderTarget& target, tgui::RenderStates states) const override {
		Container::draw(target, states);
	}

	Widget::Ptr clone() const override {
		return std::make_shared<Graph>(*this);
	}
private:
	Plot m_plot;
	float m_heightOffset;
	tgui::CanvasSFML::Ptr m_plotCanvas;
	tgui::Vector2f m_plotZoom{ 1, 1 };

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
