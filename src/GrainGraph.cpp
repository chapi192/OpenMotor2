#include "GrainGraph.hpp"
#include <cmath>
#include <iostream>

const float PI = 3.14159265;

GrainGraph::GrainGraph(tgui::Container::Ptr container, sf::Vector2f posInContainer, float length) :
	m_plot(container->getPosition(), m_grain),
	m_grainCanvas( tgui::CanvasSFML::create() )
{
	container->add(m_grainCanvas);

	m_grainCanvas->setPosition( {posInContainer.x, posInContainer.y} );
	sf::Vector2f containerSize = container->getSize();
	//float smallestLength = containerSize.x < containerSize.y ? containerSize.x : containerSize.y;
	m_grainCanvas->setSize(length, length);
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
	std::cout << "Vertices were cleared" << std::endl;
	m_plot.generateVertices(m_grain);
	std::cout << "Vertices were generated" << std::endl;
	draw();
	std::cout << "Vertices were drawn" << std::endl;
}


