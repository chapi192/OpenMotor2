#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include "Includes.h"
#include <iostream>
#include "Controllers.hpp"
#include <TGUI/Widgets/ChildWindow.hpp>
#include "GrainGraph.hpp"
#include <cmath>

float tofloat(std::string);
GrainGeometry toGrainGeometry(std::string);
std::string toLower(std::string);

int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "OpenMotor2");
    tgui::Gui gui{window};
    window.setVerticalSyncEnabled(true);

    sf::Font f_ariel;
    if (!f_ariel.loadFromFile("../data/arial.ttf")) {
        //damn...
    }

    sf::Text txt_temp;
    txt_temp.setCharacterSize(16);
    txt_temp.setFillColor(sf::Color::Black);
    txt_temp.setFont(f_ariel);
    txt_temp.setString("Hello World!");
    txt_temp.setPosition((WIDTH / 2) - (txt_temp.getGlobalBounds().width / 2), HEIGHT / 2);

    TabContainerController tcController;
    tcController.addTabContainer({ WIDTH / 40, HEIGHT / 40 }, { 77 * WIDTH / 120, 77 * HEIGHT / 120}, { "Graph", "Grain" });
    tcController.addTabContainer({ WIDTH / 40, 2 * HEIGHT / 3 }, { 19 * WIDTH / 40, 39 * HEIGHT / 120 }, { "Input" });
    tcController.addTabContainer({ WIDTH / 2, 2 * HEIGHT / 3 }, { 19 * WIDTH / 40, 39 * HEIGHT / 120 }, { "Output" });
    tcController.addTabContainer({ 2 * WIDTH / 3, HEIGHT / 40 }, { 37 * WIDTH / 120, 77 * HEIGHT / 120 }, { "File" });

    for (tgui::TabContainer::Ptr tabContainer : tcController.getTabContainers())
    {
        gui.add(tabContainer);
    }

    SeparatorLineController slController;
    slController.addSeparatorLine({ WIDTH / 40, HEIGHT / 40 }, { 19 * WIDTH / 20, 2 }, "sl_gui_border_Top", gui); // top outline
    slController.addSeparatorLine({ WIDTH / 40, 39 * HEIGHT / 40 - 1 }, { 19 * WIDTH / 20 + 1, 2 }, "sl_gui_border_Bottom", gui); // bottom outline
    slController.addSeparatorLine({ WIDTH / 40, HEIGHT / 40 }, { 2, 19 * HEIGHT / 20 }, "sl_gui_border_Left", gui); // left outline
    slController.addSeparatorLine({ 39 * WIDTH / 40 - 1, HEIGHT / 40 }, { 2, 19 * HEIGHT / 20 + 1 }, "sl_gui_border_Right", gui); // right outline
    slController.addSeparatorLine({ 2 * WIDTH / 3 - 1, HEIGHT / 40 }, { 2, 77 * HEIGHT / 120 }, "sl_gui_divider_GraphFile", gui); // graph/grain and file
    slController.addSeparatorLine({ WIDTH / 2 - 1, 2 * HEIGHT / 3 }, { 2, 37 * HEIGHT / 120 }, "sl_gui_divider_InputOuput", gui); // input and output
    slController.addSeparatorLine({ WIDTH / 40, 2 * HEIGHT / 3 }, { 19 * WIDTH / 20, 2 }, "sl_gui_divider_InputGraph", gui); // input/output and grain/graph/file
    tgui::Container::Ptr tempContainer = tcController.getTabContainers()[0]->getPanel(1);
    slController.addSeparatorLine({ tempContainer->getSize().x - tempContainer->getSize().y, 0}, 
                                  {1, tempContainer->getSize().y}, "sl_grain_divider_InputGraph", tempContainer); // grain input and grain graph

    TextInputController textInputController;

    textInputController.addTextInput({ WIDTH / 40.f + 7, 2.f / 3.f * HEIGHT + 30 }, { 50, 25 }, "ti_input_i1", gui);
    textInputController.addTextInput({ WIDTH / 40.f + 7, 2.f / 3.f * HEIGHT + 60 }, { 50, 25 }, "ti_input_i2", gui, false, "testing");
    textInputController.addTextInput({ WIDTH / 40.f + 7, 2.f / 3.f * HEIGHT + 90 }, { 50, 25 }, "ti_input_i2", gui);
    textInputController.addTextInput({ WIDTH / 40.f + 7, 2.f / 3.f * HEIGHT + 120 }, { 50, 25 }, "ti_input_i2", gui);
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

    auto tabContainer = tcController.getTabContainers()[0];
    tabContainer->select(1);
    auto container = tabContainer->getPanel(1);
    GrainGraph testGrainGraph(container, { container->getSize().x - 0.95f * container->getSize().y, container->getSize().y * 0.05f}, container->getSize().y * 0.9f);
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

    //sf::CircleShape shape(100.f);
    //shape.setFillColor(sf::Color::Green);

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


        window.draw(txt_temp);
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
