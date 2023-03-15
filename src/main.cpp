#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include "Includes.h"
#include <iostream>
#include "Controllers.hpp"
#include <TGUI/Widgets/ChildWindow.hpp>

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

    for (tgui::SeparatorLine::Ptr separatorLine : slController.getSeparatorLines())
    {
        gui.add(separatorLine);
    }

    CheckboxController cbController;
    cbController.addCheckbox({3.f / 4.f * WIDTH, 1.f / 6.f * HEIGHT}, {30, 30}, "THIS IS A TEST");
    cbController.addCheckbox({3.f / 4.f * WIDTH, 1.f / 3.f * HEIGHT}, {25, 20}, "MAKE FIRST ONE INVISIBLE!!");
    cbController.addCheckbox({3.f / 4.f * WIDTH, 1.f / 2.f * HEIGHT}, {30, 25}, "Click to close");
    cbController.addToggleInputCheckbox({3.f / 4.f * WIDTH, 2.f / 3.f * HEIGHT}, {20, 20}, cbController.getCheckBoxes()[2], "Prohibit close checkbox");

    for (tgui::CheckBox::Ptr widget : cbController.getCheckBoxes())
    {
        gui.add(widget);
    }

    TextInputController textInputController;
    textInputController.addTextInput({10, 20}, {100, 300});
    textInputController.addTextInput({150, 30}, {150, 200}, "testing");

    for (auto& textInput : textInputController.getTextInputs()) {
        gui.add(textInput);
    }

    //sf::CircleShape shape(100.f);
    //shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        window.clear(sf::Color(0xf0f0f0ff));

        sf::Event event;
        if (cbController.getCheckBoxes()[2]->isChecked())
            window.close();
        
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