/*****************************************************************//**
 * \file   ChecklistController.hpp
 * \brief  Management class for GUI Checkboxes (Add, delete, and storage)
 * 
 * \author Elijah Roell
 * \date   February 2023
 *********************************************************************/

#pragma once
#include <memory>
#include <vector>
#include "TGUI/TGUI.hpp"
#include "TGUI/Widgets/CheckBox.hpp"
#include "TGUI/Widgets/RadioButton.hpp"
#include "SFML/System/Vector2.hpp"

class CheckboxController
{
public:
	void addCheckbox(sf::Vector2f location, sf::Vector2f dimensions, std::string text = "txt");

	std::vector<tgui::CheckBox::Ptr> m_checkBoxes;

private:

};
