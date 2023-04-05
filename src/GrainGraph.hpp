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

	float retrieveGrainRadius() { return m_grain.getGrainRadius(); }

	float retrieveInnerGeometryRadius() { return m_grain.getGeometryRadiusInner(); }

	float retrieveOuterGeometryRadius() { return m_grain.getGeometryRadiusOuter(); }

	float retrieveGrainDepth() { return m_grain.getGrainDepth(); }

	int retrieveNumSpecializations() { return m_grain.getNumberOfSpecializations(); }

	std::string retrieveGrainGeometry();

	inline void updateGrainRadius(float grainRadius) { m_grain.setGrainRadius(grainRadius); 
													  update(); }

	inline void updateGrainDepth(float grainDepth) { m_grain.setGrainDepth(grainDepth); 
												    update(); }
	
	inline void updateInnerGrainRadius(float innerRadius) { m_grain.setGeometryRadiusInner(innerRadius);
														   update(); }
	
	inline void updateOuterGrainRadius(float outerRadius) { m_grain.setGeometryRadiusOuter(outerRadius);
														   update(); }
	
	inline void updateNumSpecializations(int numSpecializations) { m_grain.setNumberOfSpecializations(numSpecializations);
																  update(); }
	
	inline void updateGrainGeometry(GrainGeometry geometry) { m_grain.setGeometry(geometry);
															 update(); }

	tgui::CanvasSFML::Ptr getCanvas() const { return m_grainCanvas; }

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

