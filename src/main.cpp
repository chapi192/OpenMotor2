#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <iostream>
#include "Controllers.hpp"
#include <TGUI/Widgets/ChildWindow.hpp>
#include "GrainGraph.hpp"
#include <cmath>
#include "Graph/Graph.hpp"

#include "Motor/Motor.hpp"
#include "Motor/Output.hpp"

float tofloat(std::string);
GrainGeometry toGrainGeometry(std::string);
std::string toLower(std::string);

int main()
{
    auto desktopVideoMode = sf::VideoMode::getDesktopMode();
    const float windowScaling = 0.75;
    desktopVideoMode.width *= windowScaling;
    desktopVideoMode.height *= windowScaling;
    sf::RenderWindow window(desktopVideoMode, "OpenMotor2");
    window.setVerticalSyncEnabled(true);

    const float w = window.getSize().x;
    const float h = window.getSize().y;

    tgui::Gui gui{window};

    sf::Font f_ariel;
    auto font_path = "../data/arial.ttf";
    if (!f_ariel.loadFromFile(font_path)) {
        std::string msg = "Failed to load ";
        msg += font_path;
        throw std::runtime_error{ msg };
    }

    TabContainerController tcController;
    tcController.addTabContainer({ w / 40, h / 40 }, { 77 * w / 120, 77 * h / 120}, { "Graph", "Grain" });
    tcController.addTabContainer({ 2 * w / 3, h / 40 }, { 37 * w / 120, 77 * h / 120 }, { "File" });
    tcController.addTabContainer({ w / 40, 2 * h / 3 }, { 19 * w / 40, 37 * h / 120 }, { "Input" });
    tcController.addTabContainer({ w / 2, 2 * h / 3 }, { 19 * w / 40, 37 * h / 120 }, { "Output" });

    for (tgui::TabContainer::Ptr tabContainer : tcController.getTabContainers())
    {
        gui.add(tabContainer);
    }

    SeparatorLineController slController;
    slController.addSeparatorLine({ w / 40, h / 40 }, { 19 * w / 20, 2 }, "sl_gui_border_Top", gui); // top outline
    slController.addSeparatorLine({ w / 40, 39 * h / 40 - 1 }, { 19 * w / 20 + 1, 2 }, "sl_gui_border_Bottom", gui); // bottom outline
    slController.addSeparatorLine({ w / 40, h / 40 }, { 2, 19 * h / 20 }, "sl_gui_border_Left", gui); // left outline
    slController.addSeparatorLine({ 39 * w / 40 - 1, h / 40 }, { 2, 19 * h / 20 + 1 }, "sl_gui_border_Right", gui); // right outline
    slController.addSeparatorLine({ 2 * w / 3 - 1, h / 40 }, { 2, 77 * h / 120 }, "sl_gui_divider_GraphFile", gui); // graph/grain and file
    slController.addSeparatorLine({ w / 2 - 1, 2 * h / 3 }, { 2, 37 * h / 120 }, "sl_gui_divider_InputOuput", gui); // input and output
    slController.addSeparatorLine({ w / 40, 2 * h / 3 }, { 19 * w / 20, 2 }, "sl_gui_divider_InputGraph", gui); // input/output and grain/graph/file
    tgui::Container::Ptr tempContainer = tcController.getTabContainers()[0]->getPanel(1);
    slController.addSeparatorLine({ tempContainer->getSize().x - tempContainer->getSize().y, 0},
                                  {1, tempContainer->getSize().y}, "sl_grain_divider_InputGraph", tempContainer); // grain input and grain graph

    TextInputController textInputController;

    textInputController.addTextInput({ w / 40.f + 7, 2.f / 3.f * h + 30 }, { 50, 25 }, "ti_input_i1", gui);
    textInputController.addTextInput({ w / 40.f + 7, 2.f / 3.f * h + 60 }, { 50, 25 }, "ti_input_i2", gui, false, "testing");
    textInputController.addTextInput({ w / 40.f + 7, 2.f / 3.f * h + 90 }, { 50, 25 }, "ti_input_i2", gui);
    textInputController.addTextInput({ w / 40.f + 7, 2.f / 3.f * h + 120 }, { 50, 25 }, "ti_input_i2", gui);
    //auto tabContainer = tcController.getTabContainers()[0];
    tempContainer = tcController.getTabContainers()[0]->getPanel(1);
    float linePosX = tempContainer->get("sl_grain_divider_InputGraph")->getPosition().x;
    textInputController.addTextInput({ linePosX / 2 + 10, 20 }, { linePosX / 2 - 20, 20 }, "ti_grain_GrainRadius", tempContainer);
    textInputController.addTextInput({ linePosX / 2 + 10, 60 }, { linePosX / 2 - 20, 20 }, "ti_grain_GrainDepth", tempContainer);
    textInputController.addTextInput({ linePosX / 2 + 10, 100 }, { linePosX / 2 - 20, 20 }, "ti_grain_GrainGeometry", tempContainer);
    textInputController.addTextInput({ linePosX / 2 + 10, 140 }, { linePosX / 2 - 20, 20 }, "ti_grain_GrainGeometryInnerRadius", tempContainer);
    textInputController.addTextInput({ linePosX / 2 + 10, 180 }, { linePosX / 2 - 20, 20 }, "ti_grain_GrainGeometryOuterRadius", tempContainer);
    textInputController.addTextInput({ linePosX / 2 + 10, 220 }, { linePosX / 2 - 20, 20 }, "ti_grain_GeometryNumberOfSpecializations", tempContainer);
    textInputController.addTextInput({ linePosX / 2 + 10, 260 }, { linePosX / 2 - 20, 20 }, "ti_grain_GrainDensity", tempContainer);
    textInputController.addTextInput({ linePosX / 2 + 10, 300 }, { linePosX / 2 - 20, 20 }, "ti_grain_GrainMass", tempContainer);
    textInputController.addTextInput({ linePosX / 2 + 10, 340 }, { linePosX / 2 - 20, 20 }, "ti_grain_GraphScale", tempContainer);

    TextController tController;
    tController.addText( { 10.f, tempContainer->get("ti_grain_GrainRadius")->getPosition().y },
        "Radius:", "t_grain_GrainRadius", tempContainer, 11
    );
    tController.addText( { 10.f, tempContainer->get("ti_grain_GrainDepth")->getPosition().y },
        "Depth:", "t_grain_GrainDepth", tempContainer, 11
    );
    tController.addText( { 10.f, tempContainer->get("ti_grain_GrainGeometry")->getPosition().y },
        "Geometry:", "t_grain_GrainGeometry", tempContainer, 11
    );
    tController.addText( { 10.f, tempContainer->get("ti_grain_GrainGeometryInnerRadius")->getPosition().y },
        "Geom Inner Radius:", "t_grain_GrainGeometryInnerRadius", tempContainer, 11
    );
    tController.addText( { 10.f, tempContainer->get("ti_grain_GrainGeometryOuterRadius")->getPosition().y },
        "Geom Outer Radius:", "t_grain_GrainGeometryInnerRadius", tempContainer, 11
    );
    tController.addText( { 10.f, tempContainer->get("ti_grain_GeometryNumberOfSpecializations")->getPosition().y },
        "Num Specializations:", "t_grain_GeometryNumberOfSpecializations", tempContainer, 11
    );
    tController.addText( { 10.f, tempContainer->get("ti_grain_GrainDensity")->getPosition().y },
        "Grain Density:", "t_grain_GrainDensity", tempContainer, 11
    );
    tController.addText( { 10.f, tempContainer->get("ti_grain_GrainMass")->getPosition().y },
        "Grain Mass:", "t_grain_GrainMass", tempContainer, 11
    );
    tController.addText( { 10.f, tempContainer->get("ti_grain_GraphScale")->getPosition().y },
        "Graph Scale:", "t_grain_GraphScale", tempContainer, 11
    );

    /*auto edit = tgui::EditBox::create();
    edit->setReadOnly(true);
    edit->setPosition({ 15, tempContainer->get("ti_grain_GrainRadius")->getPosition().y });
    edit->setSize({ 100, 20 });
    edit->setText("Grain Radius:");
    edit->getRenderer()->setBackgroundColor("White");
    edit->getRenderer()->setBorderColor("White");

    tempContainer->add(edit, "t_grain_GrainRadius");*/

    CheckboxController cbController;
    cbController.addToggleInputCheckbox({ w / 40.f + 60, 2.f / 3.f * h + 38 }, { 9, 9 }, textInputController.getTextInputs()[0], "Force");
    cbController.addToggleInputCheckbox({ w / 40.f + 60, 2.f / 3.f * h + 68 }, { 9, 9 }, textInputController.getTextInputs()[1], "Input 2");
    cbController.addToggleInputCheckbox({ w / 40.f + 60, 2.f / 3.f * h + 98 }, { 9, 9 }, textInputController.getTextInputs()[2], "Input 3");
    cbController.addToggleInputCheckbox({ w / 40.f + 60, 2.f / 3.f * h + 128 }, { 9, 9 }, textInputController.getTextInputs()[3], "Input 4");

    for (tgui::CheckBox::Ptr widget : cbController.getCheckBoxes())
    {
        gui.add(widget);
    }

    ButtonController bController;
    bController.addButton({ 1.f / 2.f * w - 80, 2.f / 3.f * h + 25 }, { 75, 25 }, "Compute");

    for (tgui::Button::Ptr button : bController.getButtons())
    {
        gui.add(button);
    }

    // tab container that has the panels for visualizing data
    auto& tabContainerVisual = *tcController.getTabContainers()[0];
    auto panelGrain = tabContainerVisual.getPanel(1);
    GrainGraph testGrainGraph(panelGrain, { panelGrain->getSize().x - 0.95f * panelGrain->getSize().y, panelGrain->getSize().y * 0.05f }, panelGrain->getSize().y * 0.9f);
    testGrainGraph.updateGrainDepth(1);
    testGrainGraph.updateGrainRadius(0.5);
    testGrainGraph.updateGrainGeometry(TUBE);

    auto textBoxes = textInputController.getTextInputs();
    textBoxes[4]->onReturnKeyPress([&] { testGrainGraph.updateGrainRadius(tofloat((std::string)textBoxes[4]->getText())); }); // update grain radius
    textBoxes[5]->onReturnKeyPress([&]{ testGrainGraph.updateGrainDepth(tofloat((std::string)textBoxes[5]->getText())); }); // update grain depth
    textBoxes[6]->onReturnKeyPress([&]{ testGrainGraph.updateGrainGeometry(toGrainGeometry((std::string)textBoxes[6]->getText())); }); // update grain geometry
    textBoxes[7]->onReturnKeyPress([&]{ testGrainGraph.updateInnerGrainRadius(tofloat((std::string)textBoxes[7]->getText())); }); // update inner inner grain radius
    textBoxes[8]->onReturnKeyPress([&]{ testGrainGraph.updateOuterGrainRadius(tofloat((std::string)textBoxes[8]->getText())); }); // update outer inner grain radius
    textBoxes[9]->onReturnKeyPress([&]{ testGrainGraph.updateNumSpecializations(tofloat((std::string)textBoxes[9]->getText())); }); // update specialization amount
    textBoxes[10]->onReturnKeyPress([&]{ testGrainGraph.updateGrainDensity(tofloat((std::string)textBoxes[10]->getText())); }); // update grain density
    textBoxes[11]->onReturnKeyPress([&]{ testGrainGraph.updateGrainMass(tofloat((std::string)textBoxes[11]->getText())); }); // update grain mass
    textBoxes[12]->onReturnKeyPress([&]{ testGrainGraph.updateGraphScale(tofloat((std::string)textBoxes[12]->getText())); }); // update scale of the graph

    testGrainGraph.getCanvas()->onClick([&textBoxes, &testGrainGraph]
    {
        textBoxes[4]->setText(std::to_string(testGrainGraph.retrieveGrainRadius()));
        textBoxes[5]->setText(std::to_string(testGrainGraph.retrieveGrainDepth()));
        textBoxes[6]->setText(testGrainGraph.retrieveGrainGeometry());
        textBoxes[7]->setText(std::to_string(testGrainGraph.retrieveInnerGeometryRadius()));
        textBoxes[8]->setText(std::to_string(testGrainGraph.retrieveOuterGeometryRadius()));
        textBoxes[9]->setText(std::to_string(testGrainGraph.retrieveNumSpecializations()));
        textBoxes[10]->setText(std::to_string(testGrainGraph.retrieveGrainDensity()));
        textBoxes[11]->setText(std::to_string(testGrainGraph.retrieveGrainMass()));
        textBoxes[12]->setText(std::to_string(testGrainGraph.retrieveGraphScale()));
    } );

    motor::Motor motor;
    auto res = motor.burn();

    auto panelGraph = tabContainerVisual.getPanel(0);

    // TODO: use the same font as the one used for TGUI, instead of using f_ariel
    auto graph = graph::Graph::create(panelGraph->getSize(), f_ariel, "Time - s");
    panelGraph->add(graph);

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

    // tab container that has the panel for output
    auto& tabContainerOutput = *tcController.getTabContainers()[2];
    auto panelOutput = tabContainerOutput.getPanel(0);

    auto output = motor::Output::create(panelOutput->getSize(), res);
    panelOutput->add(output);

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

float tofloat(std::string number)
{
    float output = 0;

    int decimalIdx = number.length();

    for (int idx = 0; idx < number.length(); idx++) {
        if (number[idx] == '.') {
            decimalIdx = idx;
            break;
        }
    }
    for (int idx = 0; idx < number.length(); idx++) {
        char c = number[idx];
        if (c >= '0' && c <= '9') {
            if (idx < decimalIdx)
                output += (double)(c - 48) * pow(10, decimalIdx - idx - 1);
            else
                output += (double)(c - 48) * pow(10, decimalIdx - idx);
        }
    }

    return output;
}

GrainGeometry toGrainGeometry(std::string geom)
{
    GrainGeometry grainGeom = TUBE;
    geom = toLower(geom);

    if (geom == "tube")
        grainGeom = TUBE;
    else if (geom == "tube and rod")
        grainGeom = TUBE_AND_ROD;
    else if (geom == "star")
        grainGeom = STAR;
    else if (geom == "dog bone")
        grainGeom = DOG_BONE;
    else if (geom == "multiperforated")
        grainGeom = MULTIPERFORATED;
    else if (geom == "dendrite")
        grainGeom = DENDRITE;
    else if (geom == "wagon wheel")
        grainGeom = WAGON_WHEEL;
    else if (geom == "multifin")
        grainGeom = MULTIFIN;
    else if (geom == "double anchor")
        grainGeom = DOUBLE_ANCHOR;
    else if (geom == "grate")
        grainGeom = GRATE;
    else
        std::cout << "GrainGeometry invalid - defaulted to TUBE" << std::endl;

    return grainGeom;
}

std::string toLower(std::string str)
{
    std::string retStr = "";
    for (char c : str) {
        retStr += tolower(c);
    }
    return retStr;
}
