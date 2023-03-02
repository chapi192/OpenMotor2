#include "CheckBoxController.hpp"

void CheckboxController::addCheckbox(sf::Vector2f location, sf::Vector2f dimensions, std::string text)
{
	auto newCheckbox = tgui::CheckBox::create();
	newCheckbox->setPosition(location.x, location.y);
	newCheckbox->setWidth(dimensions.x);
	newCheckbox->setHeight(dimensions.y);
	newCheckbox->setText(text);
	newCheckbox->setVisible(true);
	m_checkBoxes.emplace_back(newCheckbox);
}

