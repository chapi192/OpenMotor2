/*****************************************************************//**
 * \file   TabContainerController.hpp
 * \brief  
 * 
 * \author Elijah Roell
 * \date   March 2023
 *********************************************************************/
 #pragma once
 
 #include <vector>
 #include <iostream>
 #include <SFML/System/Vector2.hpp>
 #include <TGUI/Widgets/TabContainer.hpp>

class TabContainerController
{
public:
	void addTabContainer(sf::Vector2f location, sf::Vector2f dimensions, std::vector<std::string> tabs);

	std::vector<tgui::TabContainer::Ptr> getTabContainers();

private:
	std::vector<tgui::TabContainer::Ptr> m_tabContainers;
};
