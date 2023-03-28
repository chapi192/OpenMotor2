#include "GrainGraph.hpp"
#include <cmath>

const float PI = 3.14159265;

GrainGraph::GrainGraph(tgui::Container::Ptr container, sf::Vector2f posInContainer, float length) :
	m_plot(container->getPosition(), m_grain),
	m_grainCanvas( tgui::CanvasSFML::create() )
{
	container->add(m_grainCanvas);

	m_grainCanvas->setPosition(container->getPosition());
	sf::Vector2f containerSize = container->getSize();
	//float smallestLength = containerSize.x < containerSize.y ? containerSize.x : containerSize.y;
	//m_grainCanvas->setSize(smallestLength, smallestLength);
}

//GrainGraph::~GrainGraph() = default;

void GrainGraph::draw()
{
	m_grainCanvas->clear(sf::Color{m_BackgroundColor});
	m_grainCanvas->draw(m_plot);
	m_grainCanvas->display();
}

void GrainGraph::setPenSize(int penSize) { m_penSize = penSize; }

int GrainGraph::getPenSize() { return m_penSize; }

void GrainGraph::update() 
{
	m_plot.clearVertices();
	m_plot.generateVertices(m_grain);
	draw();
}

void GrainGraph::updateGrain(float grainRadius, float grainDepth, float innerRadius, float outerRadius, int numSpecializations, GrainGeometry geometry)
{
	m_grain.setGrainRadius(grainRadius);
	m_grain.setGrainDepth(grainDepth);
	m_grain.setGeometryRadiusInner(innerRadius);
	m_grain.setGeometryRadiusOuter(outerRadius);
	m_grain.setNumberOfSpecializations(numSpecializations);
	m_grain.setGeometry(geometry);
}
