#pragma once
#include "Plot.hpp"
#include <TGUI/Backend/SFML-Graphics.hpp>
// #include <TGUI/Container.hpp>
#include <vector>

namespace Graph {
class Canvas {
public:
	inline Canvas(tgui::Container::Ptr container, const sf::Font& font, const std::string& xAxisLabel = "X Axis") :
			canvasSFML{ tgui::CanvasSFML::create() },
			plot{ {}, {}, 50, font, xAxisLabel }
	{
		container->add(canvasSFML);
		plot.setSize(container->getSize());
	}

	inline void addDataSet(const std::vector<float>& xAxis, const std::vector<float>& yAxis, const sf::Color& color) {
		std::string str = "Dataset " + std::to_string(plot.getDataSetCount());
		DataSet set{ xAxis, yAxis, str, color, DataSet::LINE };
		plot.addDataSet(set);
	}

	inline void update() {
		plot.scaleAxes();
		plot.generateVertices();
		draw();
	}

	inline void draw() {
		canvasSFML->clear(sf::Color{0xffffffff});
		canvasSFML->draw(plot);
		canvasSFML->display();
	}
private:
	tgui::CanvasSFML::Ptr canvasSFML;
	Plot plot;
};
}
