#include "SeparatorLineController.hpp"

void SeparatorLineController::addSeparatorLine(sf::Vector2f position, sf::Vector2f dimensions)
{
	auto newSeparatorLine = tgui::SeparatorLine::create();
	newSeparatorLine->setPosition(position.x, position.y);
	newSeparatorLine->setSize((tgui::Layout2d)dimensions);
	newSeparatorLine->getRenderer()->setColor("Black");

	m_separatorLines.emplace_back(newSeparatorLine);
}

void SeparatorLineController::addSeparatorLine(sf::Vector2f positionInContainer, sf::Vector2f dimensions, std::string name, tgui::Container::Ptr& container, tgui::Color color)
{
	auto newSeparatorLine = tgui::SeparatorLine::create();
	newSeparatorLine->setPosition(positionInContainer.x, positionInContainer.y);
	newSeparatorLine->setSize(dimensions.x, dimensions.y);
	newSeparatorLine->getRenderer()->setColor(color);
	container->add(newSeparatorLine, name);

	m_separatorLines.emplace_back(newSeparatorLine);
}

void SeparatorLineController::addSeparatorLine(sf::Vector2f positionInGUI, sf::Vector2f dimensions, std::string name, tgui::BackendGui& gui, tgui::Color color)
{
	auto newSeparatorLine = tgui::SeparatorLine::create();
	newSeparatorLine->setPosition(positionInGUI.x, positionInGUI.y);
	newSeparatorLine->setSize(dimensions.x, dimensions.y);
	newSeparatorLine->getRenderer()->setColor(color);
	gui.add(newSeparatorLine, name);

	m_separatorLines.emplace_back(newSeparatorLine);
}

std::vector<tgui::SeparatorLine::Ptr> SeparatorLineController::getSeparatorLines() { return m_separatorLines; }
