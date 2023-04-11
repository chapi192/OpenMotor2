/*****************************************************************//**
 * \file   SeparatorLineController.hpp
 * \brief  Separator Management Class
 * 
 * \author Elijah Roell
 * \date   March 2023
 *********************************************************************/

#pragma once
#include <iostream>
#include <vector>
#include <TGUI/TGUI.hpp>
#include <TGUI/Widgets/SeparatorLine.hpp>
#include <SFML/System/Vector2.hpp>

class SeparatorLineController
{
public:
	void addSeparatorLine(sf::Vector2f positionInContainer, sf::Vector2f dimensions);

	void addSeparatorLine(
		sf::Vector2f positionInContainer, 
		sf::Vector2f dimensions, 
		std::string name, 
		tgui::Container::Ptr& container, 
		tgui::Color color = {"Black"}
	);

	void addSeparatorLine(
		sf::Vector2f positionInGUI, 
		sf::Vector2f dimensions, 
		std::string name, 
		tgui::BackendGui& gui, 
		tgui::Color color = { "Black" }
	);

	std::vector<tgui::SeparatorLine::Ptr> getSeparatorLines();

private:
	std::vector<tgui::SeparatorLine::Ptr> m_separatorLines;
};
