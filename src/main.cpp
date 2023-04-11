#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include "Includes.h"
#include <iostream>
#include "Controllers.hpp"
#include <TGUI/Widgets/ChildWindow.hpp>

#include "Graph/Graph.hpp"
#include "Motor/Motor.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "OpenMotor2");
    window.setVerticalSyncEnabled(true);
    tgui::Gui gui{window};

    sf::Font f_ariel;
    auto font_path = "../data/arial.ttf";
    if (!f_ariel.loadFromFile(font_path)) {
        std::string msg = "Failed to load ";
        msg += font_path;
        throw std::runtime_error{ msg };
    }

    TabContainerController tcController;
    tcController.addTabContainer({ WIDTH / 40, HEIGHT / 40 }, { 77 * WIDTH / 120, 20 * HEIGHT / 31}, { "Graph", "Grain" });
    tcController.addTabContainer({ WIDTH / 40, 2 * HEIGHT / 3 }, { 19 * WIDTH / 40, 39 * HEIGHT / 120 }, { "Input" });
    tcController.addTabContainer({ WIDTH / 2, 2 * HEIGHT / 3 }, { 19 * WIDTH / 40, 39 * HEIGHT / 120 }, { "Output" });
    tcController.addTabContainer({ 2 * WIDTH / 3, HEIGHT / 40 }, { 37 * WIDTH / 120, 77 * HEIGHT / 120 }, { "File" });

    for (tgui::TabContainer::Ptr tabContainer : tcController.getTabContainers())
    {
        gui.add(tabContainer);
    }

    SeparatorLineController slController;
    slController.addSeparatorLine({ WIDTH / 40, HEIGHT / 40 }, { 19 * WIDTH / 20, 2 }); // top outline
    slController.addSeparatorLine({ WIDTH / 40, 39 * HEIGHT / 40 - 1 }, { 19 * WIDTH / 20 + 1, 2 }); // bottom outline
    slController.addSeparatorLine({ WIDTH / 40, HEIGHT / 40 }, { 2, 19 * HEIGHT / 20 }); // left outline
    slController.addSeparatorLine({ 39 * WIDTH / 40 - 1, HEIGHT / 40 }, { 2, 19 * HEIGHT / 20 + 1 }); // right outline
    slController.addSeparatorLine({ 2 * WIDTH / 3 - 1, HEIGHT / 40 }, { 2, 77 * HEIGHT / 120 }); // graph/grain and file
    slController.addSeparatorLine({ WIDTH / 2 - 1, 2 * HEIGHT / 3 }, { 2, 37 * HEIGHT / 120 }); // input and output
    slController.addSeparatorLine({ WIDTH / 40, 2 * HEIGHT / 3 }, { 19 * WIDTH / 20, 2 }); // input/output and grain/graph/file

    for (tgui::SeparatorLine::Ptr separatorLine : slController.getSeparatorLines())
    {
        gui.add(separatorLine);
    }


    TextInputController textInputController;
    textInputController.addTextInput({ WIDTH / 40.f + 7, 2.f / 3.f * HEIGHT + 30 }, { 50, 25 });
    textInputController.addTextInput({ WIDTH / 40.f + 7, 2.f / 3.f * HEIGHT + 60 }, { 50, 25 }, "testing");
    textInputController.addTextInput({ WIDTH / 40.f + 7, 2.f / 3.f * HEIGHT + 90 }, { 50, 25 });
    textInputController.addTextInput({ WIDTH / 40.f + 7, 2.f / 3.f * HEIGHT + 120 }, { 50, 25 });

    for (auto& textInput : textInputController.getTextInputs()) {
        gui.add(textInput);
    }

    CheckboxController cbController;
    cbController.addToggleInputCheckbox({ WIDTH / 40.f + 60, 2.f / 3.f * HEIGHT + 38 }, { 9, 9 }, textInputController.getTextInputs()[0], "Force");
    cbController.addToggleInputCheckbox({ WIDTH / 40.f + 60, 2.f / 3.f * HEIGHT + 68 }, { 9, 9 }, textInputController.getTextInputs()[1], "Input 2");
    cbController.addToggleInputCheckbox({ WIDTH / 40.f + 60, 2.f / 3.f * HEIGHT + 98 }, { 9, 9 }, textInputController.getTextInputs()[2], "Input 3");
    cbController.addToggleInputCheckbox({ WIDTH / 40.f + 60, 2.f / 3.f * HEIGHT + 128 }, { 9, 9 }, textInputController.getTextInputs()[3], "Input 4");

    for (tgui::CheckBox::Ptr widget : cbController.getCheckBoxes())
    {
        gui.add(widget);
    }

    ButtonController bController;
    bController.addButton({ 1.f / 2.f * WIDTH - 80, 2.f / 3.f * HEIGHT + 25 }, { 75, 25 }, "Compute");

    for (tgui::Button::Ptr button : bController.getButtons())
    {
        gui.add(button);
    }

    motor::Motor motor;
    auto res = motor.burn();

    auto& tabContainer = *tcController.getTabContainers()[0];
    tabContainer.select(0);  // Focuses on the "Graph" panel
    auto container = tabContainer.getPanel(0);  // A panel is a container

    // TODO: use the same font as the one used for TGUI, instead of using f_ariel
    auto graph = graph::Graph::create(container->getSize(), f_ariel, "Time - s");
    container->add(graph);

    graph->addDataSet(res.m_time, res.m_kn           , "Kn"                                  );
    graph->addDataSet(res.m_time, res.m_pressure     , "Chamber Pressure - Pa"        , false);
    graph->addDataSet(res.m_time, res.m_force        , "Thrust - N"                          );
    graph->addDataSet(res.m_time, res.m_volumeLoading, "Volume Loading - %"           , false);
    graph->addDataSet(res.m_time, res.m_exitPressure , "Nozzle Exit Pressure - Pa"    , false);
    graph->addDataSet(res.m_time, res.m_dThroat      , "Change in Throat Diameter - m", false);

    size_t massGroupID = graph->createDataSetGroup(res.m_time, "Mass of Propellant - kg", false);
    graph->addDataSetToGroup(res.m_grains[0].m_mass, massGroupID, " - G1");
    graph->addDataSetToGroup(res.m_grains[1].m_mass, massGroupID, " - G2");
    graph->addDataSetToGroup(res.m_grains[2].m_mass, massGroupID, " - G3");
    size_t flowGroupID = graph->createDataSetGroup(res.m_time, "Mass flow - kg/s"       , false);
    graph->addDataSetToGroup(res.m_grains[0].m_massFlow, flowGroupID, " - G1");
    graph->addDataSetToGroup(res.m_grains[1].m_massFlow, flowGroupID, " - G2");
    graph->addDataSetToGroup(res.m_grains[2].m_massFlow, flowGroupID, " - G3");
    size_t fluxGroupID = graph->createDataSetGroup(res.m_time, "Mass flux - kg/(s*m^2)");
    graph->addDataSetToGroup(res.m_grains[0].m_massFlux, fluxGroupID, " - G1");
    graph->addDataSetToGroup(res.m_grains[1].m_massFlux, fluxGroupID, " - G2");
    graph->addDataSetToGroup(res.m_grains[2].m_massFlux, fluxGroupID, " - G3");
    size_t regGroupID = graph->createDataSetGroup(res.m_time, "Regression Depth - m"    , false);
    graph->addDataSetToGroup(res.m_grains[0].m_reg, regGroupID, " - G1");
    graph->addDataSetToGroup(res.m_grains[1].m_reg, regGroupID, " - G2");
    graph->addDataSetToGroup(res.m_grains[2].m_reg, regGroupID, " - G3");
    size_t webGroupID = graph->createDataSetGroup(res.m_time, "Web - m"                 , false);
    graph->addDataSetToGroup(res.m_grains[0].m_web, webGroupID, " - G1");
    graph->addDataSetToGroup(res.m_grains[1].m_web, webGroupID, " - G2");
    graph->addDataSetToGroup(res.m_grains[2].m_web, webGroupID, " - G3");
    graph->update();

    while (window.isOpen())
    {
        window.clear(sf::Color(0xf0f0f0ff));

        sf::Event event;

        while (window.pollEvent(event))
        {
            gui.handleEvent(event);

            if (event.type == sf::Event::Closed)
                window.close();
        }

        gui.draw();
        window.display();
    }

    return 0;
}