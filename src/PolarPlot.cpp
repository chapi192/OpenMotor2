#include "PolarPlot.hpp"
#include <iostream>
#include <cmath>

const float PI = 3.14159265;

PolarPlot::PolarPlot(
	sf::Vector2f position,
	Grain& grain,
	float length, 
	float margin,  
	const sf::Color& plot, 
	const sf::Color& vertex
) :
		m_position(position),
		m_size({length, length}), 
		m_margin(margin), 
		m_plotColor(plot), 
		m_vertexColor(vertex)
{ 
	m_center = { position.x - m_size.x / 2, position.y - m_size.y / 2 }; 
	m_slices = (length - 2 * margin) / ( 10.f / 9.f );
	m_vertexColor = sf::Color{0x00FF00};
	sf::RectangleShape shape(m_size);
	shape.setPosition(m_position);
	shape.setOutlineColor(sf::Color::Black);
	shape.setOutlineThickness(2);
}

void PolarPlot::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_vertices);
}

void PolarPlot::generateVertices(Grain& grain)
{
	calculatePolarCoords(grain);
	toCartesianCoords(m_polarVertexPosition);
}

void PolarPlot::calculatePolarCoords(Grain& grain)
{
	float plotSize = m_size.x - 2.f * m_margin;
	for (int slice = 0; slice < m_slices; slice++) {
		sf::Vector2f polarCoord;
		polarCoord.x = plotSize;
		polarCoord.y = (2 * PI - (2 * PI / m_slices) ) * slice;
		m_polarVertexPosition.emplace_back(polarCoord);
	}
	int slicesInner = (m_size.x - 2 * m_margin) * (grain.getGeometryRadiusInner() / grain.getGrainRadius()) * (10.f / 9.f);
	int slicesOuter = (m_size.x - 2 * m_margin) * (grain.getGeometryRadiusOuter() / grain.getGrainRadius()) * (10.f / 9.f);
	switch (grain.getGrainGeometry()) {
	case (GrainGeometry::TUBE):
		for (int slice = 0; slice < slicesOuter; slice++) {
			sf::Vector2f polarCoord;
			polarCoord.x = plotSize * (grain.getGeometryRadiusOuter() / grain.getGrainRadius());
			polarCoord.y = (2 * PI - (2 * PI / slicesOuter) ) * slice;
			m_polarVertexPosition.emplace_back(polarCoord);
		}
		break;
	case (GrainGeometry::TUBE_AND_ROD):
		for (int slice = 0; slice < slicesOuter; slice++) {
			sf::Vector2f polarCoord;
			polarCoord.x = plotSize * (grain.getGeometryRadiusOuter() / grain.getGrainRadius());
			polarCoord.y = (2 * PI - (2 * PI / slicesOuter)) * slice;
			m_polarVertexPosition.emplace_back(polarCoord);
		}
		for (int slice = 0; slice < slicesInner; slice++) {
			sf::Vector2f polarCoord;
			polarCoord.x = plotSize * (grain.getGeometryRadiusInner() / grain.getGrainRadius());
			polarCoord.y = (2 * PI - (2 * PI / slicesInner)) * slice;
			m_polarVertexPosition.emplace_back(polarCoord);

		}
		break;
	default:
		std::cout << "INVALID GRAIN GEOMETRY" << std::endl;
		break;
	}
}

void PolarPlot::toCartesianCoords(std::vector<sf::Vector2f>& coords )
{
	for (sf::Vector2f polarCoord : coords) {
		sf::Vertex cartesianCoord({m_center.x + polarCoord.x * cos(polarCoord.y), m_center.y + polarCoord.x * sin(polarCoord.y)}, m_vertexColor);
		m_vertices.append(cartesianCoord);
	}
}
