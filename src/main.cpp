#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include "CheckBoxController.hpp"
#include "Includes.h"
#include <iostream>

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

    CheckboxController cbController;
    try {
    cbController.addCheckbox(sf::Vector2f(3.f / 4.f * WIDTH, 1.f / 6.f * HEIGHT), sf::Vector2f(30, 30), "THIS IS A TEST");
    cbController.addCheckbox(sf::Vector2f(3.f / 4.f * WIDTH, 1.f / 3.f * HEIGHT), sf::Vector2f(25, 20), "MAKE FIRST ONE INVISIBLE!!");
    cbController.addCheckbox(sf::Vector2f(3.f / 4.f * WIDTH, 1.f / 2.f * HEIGHT), sf::Vector2f(30, 25), "Click to close");
    cbController.addCheckbox(sf::Vector2f(3.f / 4.f * WIDTH, 2.f / 3.f * HEIGHT), sf::Vector2f(20, 20), "Prohibit close checkbox");
    }
    catch (const tgui::Exception& e) {
        std::cerr << "TGUI Exception: " << e.what() << std::endl;
    }
    for (tgui::CheckBox::Ptr widget : cbController.m_checkBoxes)
    {
        gui.add(widget);
    }
    //sf::CircleShape shape(100.f);
    //shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        window.clear(sf::Color(0x0f0f0fff));

        sf::Event event;
        if (cbController.m_checkBoxes[2]->isChecked())
            window.close();
        if (cbController.m_checkBoxes[1]->isChecked())
            cbController.m_checkBoxes[0]->setVisible(false);
        else 
            cbController.m_checkBoxes[0]->setVisible(true);
        if (cbController.m_checkBoxes[3]->isChecked())
            cbController.m_checkBoxes[2]->setEnabled(false);
        else 
            cbController.m_checkBoxes[2]->setEnabled(true);

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