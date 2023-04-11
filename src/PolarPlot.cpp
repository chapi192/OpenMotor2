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
		m_size({(float)(int)length, (float)(int)length}), 
		m_margin(margin), 
		m_plotColor(plot), 
		m_vertexColor(vertex)
{ 
	m_center = { m_size.x / 2, m_size.y / 2 }; 
	m_slices = (length - 2 * margin) / ( 10.f / 9.f );
	m_vertexColor = sf::Color::Black;

	m_border.setSize({m_size.x - 2, m_size.y - 2});
	m_border.setPosition( position.x + 1, 1 );
	m_border.setPosition( 1, 1 );
	m_border.setOutlineColor(sf::Color::Black);
	m_border.setFillColor(sf::Color::Transparent);
	m_border.setOutlineThickness(1);
	m_vertices.setPrimitiveType(sf::LineStrip);
}

void PolarPlot::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_vertices);
	target.draw(m_border);
}

void PolarPlot::generateVertices(Grain& grain)
{
	calculatePolarCoords(grain);
	toCartesianCoords(m_polarVertexPosition);
}

void PolarPlot::calculatePolarCoords(Grain& grain)
{
	float plotSize = m_size.x - 2.f * m_margin;
	for (int slice = 0; slice <= m_slices; slice++) {
		sf::Vector2f polarCoord;
		polarCoord.x = plotSize / 2;
		polarCoord.y = (2 * PI - (2 * PI / m_slices) ) * slice;
		m_polarVertexPosition.emplace_back(polarCoord);
	}
	int slicesInner = (m_size.x - 2 * m_margin) * (grain.getGeometryRadiusInner() / grain.getGrainRadius()) * (10.f / 9.f);
	int slicesOuter = (m_size.x - 2 * m_margin) * (grain.getGeometryRadiusOuter() / grain.getGrainRadius()) * (10.f / 9.f);
	switch (grain.getGrainGeometry()) {
	case (TUBE):
		for (int slice = 0; slice <= slicesOuter; slice++) {
			sf::Vector2f polarCoord;
			polarCoord.x = (plotSize / 2) * (grain.getGeometryRadiusOuter() / grain.getGrainRadius());
			polarCoord.y = (2 * PI - (2 * PI / slicesOuter) ) * slice;
			m_polarVertexPosition.emplace_back(polarCoord);
		}
		break;
	case (TUBE_AND_ROD):
		for (int slice = 0; slice <= slicesOuter; slice++) {
			sf::Vector2f polarCoord;
			polarCoord.x = (plotSize / 2) * (grain.getGeometryRadiusOuter() / grain.getGrainRadius());
			polarCoord.y = (2 * PI - (2 * PI / slicesOuter)) * slice;
			m_polarVertexPosition.emplace_back(polarCoord);
		}
		for (int slice = 0; slice <= slicesInner; slice++) {
			sf::Vector2f polarCoord;
			polarCoord.x = (plotSize / 2) * (grain.getGeometryRadiusInner() / grain.getGrainRadius());
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
