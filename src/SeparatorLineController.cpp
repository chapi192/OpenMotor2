#include "SeparatorLineController.hpp"

void SeparatorLineController::addSeparatorLine(sf::Vector2f position, sf::Vector2f dimensions)
{
	auto newSeparatorLine = tgui::SeparatorLine::create();
	newSeparatorLine->setPosition(position.x, position.y);
	newSeparatorLine->setSize(dimensions.x, dimensions.y);
	newSeparatorLine->getRenderer()->setColor("Black");

	m_separatorLines.emplace_back(newSeparatorLine);
}

std::vector<tgui::SeparatorLine::Ptr> SeparatorLineController::getSeparatorLines() { return m_separatorLines; }
