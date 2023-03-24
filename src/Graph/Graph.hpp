#pragma once
#include "Plot.hpp"
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Widgets/ChildWindow.hpp>
#include <TGUI/Widgets/VerticalLayout.hpp>
#include <TGUI/Widgets/ToggleButton.hpp>
#include <vector>

namespace Graph {
class Graph {
public:
	inline Graph(tgui::Container::Ptr container, const sf::Font& font, const std::string& xAxisLabel = "X Axis"):
			canvasSFML{ tgui::CanvasSFML::create() },
			plot{ {}, {}, 50, font, xAxisLabel }
	{
		container->add(canvasSFML);
		plot.setSize(container->getSize());

		legend = tgui::ChildWindow::create("Legend", 0);
		legend->setPosition(0.75 * container->getSize().x, 0.05 * container->getSize().y);
		container->add(legend);

		layout = tgui::VerticalLayout::create();
		legend->add(layout);
	}

	inline void addDataSet(const std::vector<float>& xAxis, const std::vector<float>& yAxis, const sf::Color& color, bool on = true) {
		const int datasetID = plot.getDataSetCount();
		std::string str = "Dataset " + std::to_string(datasetID);
		DataSet set{ xAxis, yAxis, str, color, DataSet::LINE };
		plot.addDataSet(set);

		auto button = tgui::ToggleButton::create(str, on);
		if (!on)
			toggleDataset(datasetID);
		button->onToggle(&toggleDataset, this, datasetID);

		auto size = button->getSize();
		size.y += 10;
		layout->add(button);
		legend->setSize(size.x, layout->getWidgets().size() * size.y);
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

	inline void toggleDataset(int index) {
		plot.toggleDatasetVisibility(index);
		draw();
	}
private:
	tgui::CanvasSFML::Ptr canvasSFML;
	tgui::ChildWindow::Ptr legend;
	tgui::VerticalLayout::Ptr layout;
	Plot plot;
};
}
