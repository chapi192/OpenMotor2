#include "ButtonController.hpp"

void ButtonController::addButton(sf::Vector2f location, sf::Vector2f dimensions, std::string text)
{
	auto newButton = tgui::Button::create();
	newButton->setPosition(location.x, location.y);
	newButton->setSize(dimensions.x, dimensions.y);
	newButton->setText(text);
	newButton->setVisible(true);

	m_buttons.emplace_back(newButton);
}

std::vector<tgui::Button::Ptr> ButtonController::getButtons() { return m_buttons; }
