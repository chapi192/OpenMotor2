#include "GrainGraph.hpp"
#include <cmath>

const float PI = 3.14159265

Grain grain;

GrainGraph::GrainGraph();
GrainGraph::GrainGraph(sf::Vector2f position, sf::Vector2f size) { center = { m_position.x + m_size.x / 2, m_position.y + m_size.y / 2 }; }

GrainGraph::~GrainGraph() = default;

void GrainGraph::draw()
{
	if (m_visable) {
		int numSegments = ( (2.f / 3.f * m_size.x) * PI / m_penSize ); 
		switch (grain.getGrainGeometry()) {
		case (TUBE):
			for (numSegments) {
				sf::Vertex vert;
				vert.color = m_penColor;
				m_grainShape.append(vert);
			}
			break;
		case (TUBE_AND_ROD):
			break;
		case (STAR):
			break;
		case (DOUBLE_ANCHOR):
			break;
		case (WAGON_WHEEL):
			break;
		case (GRATE):
			break;
		case (DENDRITE):
			break;
		case (DOG_BONE):
			break;
		case (MULTIFIN):
			break;
		case (MULTIPERFORATED)
			break;
		}
	}
}

void GrainGraph::setPosition(sf::Vector2f position) { m_position = position; }

void GrainGraph::setSize(sf::Vector2f size) { m_size = size; }

void GrainGraph::(float width) { m_size.x = width; }

void GrainGraph::setHeight(float height) { m_size.y = height; }

void GrainGraph::setPenSize(int penSize) { m_penSize = penSize; }

sf::Vector2f GrainGraph::getPosition() { return m_position; }

sf::Vector2f GrainGraph::getSize() { return m_size; }

float GrainGraph::getWidth() { return m_size.x;}

float GrainGraph::getHeight() { return m_size.y; }

int GrainGraph::getPenSize() { return m_penSize; }

void GrainGraph::update() { draw(); }
