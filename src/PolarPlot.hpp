#pragma once

#include <vector>
#include <Grain.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Drawable.hpp>

class PolarPlot : public sf::Drawable
{
public:
	PolarPlot(sf::Vector2f position, Grain& grain, float length = 50, float margin = 5, const sf::Color& plot = sf::Color::Green, const sf::Color& vertex = sf::Color::Black);

	void generateVertices(Grain& grain);

	inline void clearVertices() { m_polarVertexPosition.clear(); 
								  m_vertices.clear(); }

	void setSize(float length) { m_size = { length, length }; }
	
	void setPosition(sf::Vector2f position) { m_position = {position.x, position.y}; }

	void setPlotColor(sf::Color color) { m_plotColor = color; }

	sf::Vector2f getSize() const { return m_size; }

	sf::Vector2f getPosition() const { return m_position; }

	sf::Color getPlotColor() const { return m_plotColor; }

	sf::Color getVertexColor() const { return m_vertexColor; }

private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void calculatePolarCoords(Grain& grain);

	void toCartesianCoords(std::vector<sf::Vector2f>& polarCoordinates);

private:
	float m_margin;

	int m_slices;

	std::vector<sf::Vector2f> m_polarVertexPosition; // position of vertex from the origin of the PolarPlot {r, theta}
	
	sf::Vector2f m_size;
	sf::Vector2f m_position;
	sf::Vector2f m_center;

	sf::Color m_plotColor;
	sf::Color m_vertexColor;

	sf::VertexArray m_vertices;
};
