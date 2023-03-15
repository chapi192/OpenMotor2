#include "CheckBoxController.hpp"
#include <iostream>

void CheckboxController::addCheckbox(sf::Vector2f location, sf::Vector2f dimensions, std::string text)
{
	auto newCheckbox = tgui::CheckBox::create();
	newCheckbox->setPosition(location.x, location.y);
	newCheckbox->setSize(dimensions.x, dimensions.y);
	newCheckbox->setText(text);
	newCheckbox->setVisible(true);

	newCheckbox->onChange(callback_onChange, newCheckbox);

	m_checkBoxes.emplace_back(newCheckbox);
}

void CheckboxController::addToggleInputCheckbox(sf::Vector2f location, sf::Vector2f dimensions, tgui::Widget::Ptr widget, std::string text)
{
	auto newCheckbox = tgui::CheckBox::create();
	newCheckbox->setPosition(location.x, location.y);
	newCheckbox->setSize(dimensions.x, dimensions.y);
	newCheckbox->setText(text);

	newCheckbox->onChange(callback_onToggleInput, newCheckbox, widget);

	m_checkBoxes.emplace_back(newCheckbox);
}

std::vector<tgui::CheckBox::Ptr> CheckboxController::getCheckBoxes() { return m_checkBoxes; }

void CheckboxController::callback_onChange(tgui::CheckBox::Ptr checkBox) 
{
	std::cout << "CheckBox toggled: " << checkBox->getText() << std::endl;
}

void CheckboxController::callback_onToggleInput(tgui::CheckBox::Ptr checkbox, tgui::Widget::Ptr widget)
{
	if (checkbox->isChecked())
		widget->setEnabled(false);
	else
		widget->setEnabled(true);
}

