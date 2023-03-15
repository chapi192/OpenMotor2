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
	void addSeparatorLine(sf::Vector2f position, sf::Vector2f dimensions);

	std::vector<tgui::SeparatorLine::Ptr> getSeparatorLines();

private:
	std::vector<tgui::SeparatorLine::Ptr> m_separatorLines;
};
