#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include "Includes.h"
#include <iostream>
#include "Controllers.hpp"
#include <TGUI/Widgets/ChildWindow.hpp>
#include "GrainGraph.hpp"

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
    tcController.addTabContainer({ WIDTH / 40, HEIGHT / 40 }, { 77 * WIDTH / 120, 2 * HEIGHT / 3}, { "Graph", "Grain" });
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
    auto tempContainer = tcController.getTabContainers()[0];
    slController.addSeparatorLine({0.25f * tempContainer->getSize().x + tempContainer->getPosition().x, tempContainer->getPosition().y + tempContainer->getTabs()->getSize().y},
                                  {1, tempContainer->getSize().y - tempContainer->getTabs()->getSize().y});
    
    tempContainer->select(1);
    tempContainer->getPanel(1)->add(slController.getSeparatorLines()[7]);


    for (tgui::SeparatorLine::Ptr separatorLine : slController.getSeparatorLines())
    {
        gui.add(separatorLine);
    }

    TextInputController textInputController;
    textInputController.addTextInput({ WIDTH / 40.f + 7, 2.f / 3.f * HEIGHT + 30 }, { 50, 25 });
    textInputController.addTextInput({ WIDTH / 40.f + 7, 2.f / 3.f * HEIGHT + 60 }, { 50, 25 }, "testing");
    textInputController.addTextInput({ WIDTH / 40.f + 7, 2.f / 3.f * HEIGHT + 90 }, { 50, 25 });
    textInputController.addTextInput({ WIDTH / 40.f + 7, 2.f / 3.f * HEIGHT + 120 }, { 50, 25 });
    tempContainer = tcController.getTabContainers()[0];
    textInputController.addTextInput({ tempContainer->getPosition().x + 15, tempContainer->getPosition().y + 30 }, { 75, 20 });
    textInputController.addTextInput({ tempContainer->getPosition().x + 15, tempContainer->getPosition().y + 60 }, { 75, 20 });
    textInputController.addTextInput({ tempContainer->getPosition().x + 15, tempContainer->getPosition().y + 90 }, { 75, 20 });
    textInputController.addTextInput({ tempContainer->getPosition().x + 15, tempContainer->getPosition().y + 120 }, { 75, 20 });
    textInputController.addTextInput({ tempContainer->getPosition().x + 15, tempContainer->getPosition().y + 150 }, { 75, 20 });
    textInputController.addTextInput({ tempContainer->getPosition().x + 15, tempContainer->getPosition().y + 180 }, { 75, 20 });
    textInputController.addTextInput({ tempContainer->getPosition().x + 15, tempContainer->getPosition().y + 210 }, { 75, 20 });
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

    auto tabContainer = tcController.getTabContainers()[0];
    tabContainer->select(1);
    auto container = tabContainer->getPanel(1);
    GrainGraph testGrainGraph(container, { 0.25f * container->getSize().x + 20, 20}, 100);
    testGrainGraph.updateGrainDepth(1);
    testGrainGraph.updateGrainRadius(0.5);
    testGrainGraph.updateGrainGeometry(TUBE);

    auto textBoxes = textInputController.getTextInputs();
    textBoxes[4]->onTextChange([&]{ testGrainGraph.updateGrainRadius(tofloat((std::string)textBoxes[4]->getText())); }); // update grain radius
    textBoxes[5]->onTextChange([&]{ testGrainGraph.updateGrainDepth(tofloat((std::string)textBoxes[5]->getText())); }); // update grain depth
    textBoxes[6]->onTextChange([&]{ testGrainGraph.updateGrainGeometry(toGrainGeometry((std::string)textBoxes[6]->getText())); }); // update grain geometry
    textBoxes[7]->onTextChange([&]{ testGrainGraph.updateInnerGrainRadius(tofloat((std::string)textBoxes[7]->getText())); }); // update inner inner grain radius
    textBoxes[8]->onTextChange([&]{ testGrainGraph.updateOuterGrainRadius(tofloat((std::string)textBoxes[8]->getText())); }); // update outer inner grain radius
    textBoxes[9]->onTextChange([&]{ testGrainGraph.updateNumSpecializations(tofloat((std::string)textBoxes[9]->getText())); }); // update specialization amount

    testGrainGraph.getCanvas()->onClick([&textBoxes, &testGrainGraph]
    {
        textBoxes[4]->setText(std::to_string(testGrainGraph.retrieveGrainRadius()));
        textBoxes[5]->setText(std::to_string(testGrainGraph.retrieveGrainDepth()));
        textBoxes[6]->setText(testGrainGraph.retrieveGrainGeometry());
        textBoxes[7]->setText(std::to_string(testGrainGraph.retrieveInnerGeometryRadius()));
        textBoxes[8]->setText(std::to_string(testGrainGraph.retrieveOuterGeometryRadius()));
        textBoxes[9]->setText(std::to_string(testGrainGraph.retrieveNumSpecializations()));
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
