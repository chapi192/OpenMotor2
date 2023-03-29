#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include "Includes.h"
#include <iostream>
#include "Controllers.hpp"
#include <TGUI/Widgets/ChildWindow.hpp>

#include "Graph/Graph.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "OpenMotor2");
    tgui::Gui gui{window};

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

    auto& tabContainer = *tcController.getTabContainers()[0];
    tabContainer.select(0);  // To select the "Graph" panel
    auto container = tabContainer.getPanel(0);  // A panel is a container

    // TODO: use the same font as the one used for TGUI, instead of using f_ariel
    graph::Graph graph{ container, f_ariel, "Time" };

    std::vector<float> xAxis = { 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
    std::vector<float> yAxis0 = { 6, 8, 12, 4, 5, 6, 5, 6, 7, 8 };
    std::vector<float> yAxis1 = { 5, 7, 8, 6, 4, 7, 8, 9, 11, 15 };
    std::vector<float> yAxis2 = { 2, 8, 5, 12, 3, 17, 3, 2, 5, 3 };
    std::vector<float> yAxis3 = { 3, 5, 3, 6, 7, 9, 5, 7, 6, 8 };
    graph.addDataSet(xAxis, yAxis0, "Data 0");
    graph.addDataSet(xAxis, yAxis1, "Data 1 long string");
    graph.addDataSet(xAxis, yAxis2, "Data 2 longest string for example");
    graph.addDataSet(xAxis, yAxis3, "Data 3 shorter", 0);
    graph.update();

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