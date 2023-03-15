/*****************************************************************//**
 * \file   ChecklistController.hpp
 * \brief  Management class for GUI Checkboxes (Add, delete, and storage)
 * 
 * \author Elijah Roell
 * \date   February 2023
 *********************************************************************/

#pragma once
#include <vector>
#include "TGUI/TGUI.hpp"
#include "TGUI/Widgets/CheckBox.hpp"
#include "SFML/System/Vector2.hpp"

class CheckboxController
{
public:
	/**
	 * @brief Creates CheckBox that sends out if toggled to the terminal.
	 * @param location The x and y float values input as a sf::Vector2f for the position of the text box based on the top left corner of the window
	 * @param dimensions The length and width of the CheckBox input as a sf::Vector2f
	 * @param text The text label of the CheckBox
	 */
	void addCheckbox(sf::Vector2f location, sf::Vector2f dimensions, std::string text = "txt");

	/**
	 * @brief Creates CheckBox that toggles input for a Widget.
	 * @param location The x and y float values input as a sf::Vector2f for the position of the text box based on the top left corner of the window
	 * @param dimensions The length and width of the CheckBox input as a sf::Vector2f 
	 * @param widget The widget having input toggled on and off
	 * @param text The text label of the CheckBox
	 */
	void addToggleInputCheckbox(sf::Vector2f location, sf::Vector2f dimensions, tgui::Widget::Ptr widget, std::string text = "txt");

	/**
	 * @brief allows access to the member vector m_checkBoxes.
	 * 
	 * \return m_checkBoxes
	 */
	std::vector<tgui::CheckBox::Ptr> getCheckBoxes();

	static void callback_onChange(tgui::CheckBox::Ptr checkBox);

	static void callback_onToggleInput(tgui::CheckBox::Ptr checkbox, tgui::Widget::Ptr widget);	

private:
	std::vector<tgui::CheckBox::Ptr> m_checkBoxes;

};
