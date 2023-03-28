#pragma once
#include <vector>
#include "Grain.hpp"
#include "PolarPlot.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>
#include <TGUI/Widgets/ChildWindow.hpp>
#include <TGUI/Widgets/TabContainer.hpp>
#include <TGUI/Backend/Renderer/SFML-Graphics/CanvasSFML.hpp>

class GrainGraph
{
public:

	//GrainGraph();
	GrainGraph(tgui::Container::Ptr container, sf::Vector2f posInContainer, float length = 50);

	~GrainGraph() = default;
	 
	void draw();

	void setPenSize(int penSize);

	int getPenSize();

	void update();

	void updateGrain(float grainRadius, float grainDepth, float innerRadius, float outerRadius, int numSpecializations, GrainGeometry geometry);

private:
	Grain m_grain;
	PolarPlot m_plot;

	tgui::CanvasSFML::Ptr m_grainCanvas;
	
	bool m_visible = true;
	int m_penSize = 1;

	sf::VertexArray m_grainShape;

	sf::Color m_penColor;
	sf::Color m_BackgroundColor;
};

