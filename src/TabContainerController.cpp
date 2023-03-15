#include "TabContainerController.hpp"

void TabContainerController::addTabContainer(sf::Vector2f location, sf::Vector2f dimensions, std::vector<std::string> tabs)
{
    auto tabContainer = tgui::TabContainer::create();
    for (std::string label : tabs)
        tabContainer->addTab(label);
    tabContainer->setPosition(location.x, location.y);
    tabContainer->setSize(dimensions.x, dimensions.y);

    m_tabContainers.emplace_back(tabContainer);
}

std::vector<tgui::TabContainer::Ptr> TabContainerController::getTabContainers() { return m_tabContainers; }