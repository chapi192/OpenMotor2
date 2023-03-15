/*****************************************************************//**
 * \file   ButtonController.hpp
 * \brief  Button Management class
 * 
 * \author Elijah Roell
 * \date   March 2023
 *********************************************************************/
#pragma once
#include <iostream>
#include <vector>
#include <TGUI/TGUI.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <SFML/System/Vector2.hpp>

class ButtonController
{
public:
	void addButton(sf::Vector2f location, sf::Vector2f dimensions, std::string text = "txt");

	std::vector<tgui::Button::Ptr> getButtons();

private:
	std::vector<tgui::Button::Ptr> m_buttons;
};
