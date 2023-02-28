#include <SFML/Graphics.hpp>
#include <iostream>
#include "Includes.h"
#include "ui_Button.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "OpenMotor2");
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

    int btn_test_id = UIButtonHelper::createButton({ 10, 10 }, TINY, sf::Color(0xaeaeae), "Tiny");
    UIButtonHelper::createButton({ 10, 40 }, SMALL, sf::Color(0xaeaeae), "Small");
    UIButtonHelper::createButton({ 10, 80 }, MEDIUM, sf::Color(0xaeaeae), "Medium");
    UIButtonHelper::createButton({ 10, 130 }, LARGE, sf::Color(0xaeaeae), "Large");
    UIButtonHelper::createButton({ 10, 200 }, MASSIVE, sf::Color(0xaeaeae), "Massive");

    int btn_test0_id = UIButtonHelper::createExactButton({ 10,290 }, { 100, 30 }, sf::Color(0xa0a0a0a0), "Custom", 14);

    int btn_test_resize_id = UIButtonHelper::createExactButton({ 10,330 }, { 100, 30 }, sf::Color(0xa0a0a0a0), "Resize", 14);

    std::cout << "max resolution: " <<  MAX_WIDTH << ", " << MAX_HEIGHT << std::endl;
    std::cout << "current resolution: " << WIDTH << ", " << HEIGHT << std::endl;

    while (window.isOpen())
    {
        window.clear(sf::Color(0xf0f0f0ff));

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonReleased && event.key.code == sf::Mouse::Left) {
                for (auto btn : UIButtonHelper::list) {
                    //bool mutex = false;
                    btn->m_mutex = false;
                }
            }
            if (event.type == sf::Event::Resized) {
                window.setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
                WIDTH = window.getSize().x;
                HEIGHT = window.getSize().y;
                std::cout << "new resolution: " << WIDTH << ", " << HEIGHT << std::endl;
            }


        }



        /*
        this cute little segment iterates through every button and tests its activation prerequisites
        if we are clicking on a button we can do some logic here
        
        todo:
        maybe push this somewhere else and make it a function, its really gross to look at 
        and will get super long when we start adding more buttons.

        just some food for thought.
        */
        for (auto btn : UIButtonHelper::list) {
            btn->update((sf::Vector2f)sf::Mouse::getPosition(window));
            btn->render(window);

            //if the current button is our test button
            if (btn->id == btn_test_id) {
                //create a temporary reference to the button
                auto btn_test = UIButtonHelper::list.at(btn_test_id);

                //if we are clicking the button and the mutex lock is not on
                if (btn_test->isClicked() && !btn_test->m_mutex) {
                    //perform button functionality
                    //.......

                    std::cout << "this is a test button!" << std::endl;

                    //.......
                    
                    //turn the mutex lock on so this doesnt keep activating every frame
                    btn_test->m_mutex = true;
                }
            }

            else if (btn->id == btn_test_resize_id) {
                auto btn_test_resize = UIButtonHelper::list.at(btn_test_resize_id);

                if (btn_test_resize->isClicked() && !btn_test_resize->m_mutex) {
                    std::cout << "yayyy" << std::endl;
                    window.setPosition({ 0,0 });
                    window.setSize({ MAX_WIDTH, MAX_HEIGHT });
                    btn_test_resize->m_mutex = true;
                }
            }

        }

        window.draw(txt_temp);
        window.display();
    }

    return 0;
}