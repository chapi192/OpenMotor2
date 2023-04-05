#include "GrainGraph.hpp"
#include <cmath>
#include <iostream>

const float PI = 3.14159265;

GrainGraph::GrainGraph(tgui::Container::Ptr container, sf::Vector2f posInContainer, float length) :
	m_plot(container->getPosition(), m_grain, length),
	m_grainCanvas( tgui::CanvasSFML::create() )
{
	container->add(m_grainCanvas);

	m_grainCanvas->setPosition( {posInContainer.x, posInContainer.y} );
	sf::Vector2f containerSize = container->getSize();
	//float smallestLength = containerSize.x < containerSize.y ? containerSize.x : containerSize.y;
	m_grainCanvas->setSize(length, length);
	m_BackgroundColor = sf::Color{0xf0f0f0ff};
}

std::string GrainGraph::retrieveGrainGeometry()
{
	std::string output = "";
	switch(m_grain.getGrainGeometry()) {
	case(DENDRITE):
		output = "Dendrite";
		break;
	case(DOG_BONE):
		output = "Dog Bone";
		break;
	case(DOUBLE_ANCHOR):
		output = "Double Anchor";
		break;
	case(GRATE):
		output = "Grate";
		break;
	case(MULTIFIN):
		output = "Multifin";
		break;
	case(MULTIPERFORATED):
		output = "Multiperforated";
		break;
	case(STAR):
		output = "Star";
		break;
	case(TUBE):
		output = "Tube";
		break;
	case(TUBE_AND_ROD):
		output = "Tube and Rod";
		break;
	case(WAGON_WHEEL):
		output = "Wagon Wheel";
		break;
	}

	return output;
}

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


