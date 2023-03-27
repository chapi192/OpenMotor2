#pragma once
#include <vector>
#include "Grain.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>

class GrainGraph
{
public:
	const sf::Vector2f center;
	Grain grain;

	GrainGraph();
	GrainGraph(sf::Vector2f position, sf::Vector2f size);

	~GrainGraph() = default;

	void draw();

	void setPosition(sf::Vector2f position);
	void setSize(sf::Vector2f size);
	void setWidth(float width);
	void setHeight(float height);
	void setPenSize(int penSize);

	sf::Vector2f getPosition();
	sf::Vector2f getSize();
	float getWidth();
	float getHeight();
	int getPenSize();

	void update();

private:
	bool m_visable;
	sf::Vector2f m_position;
	sf::Vector2f m_size;
	int m_penSize;
	sf::Color m_penColor;
	sf::Color m_BackgroundColor;

	sf::Vertex m_grainShape;
};

