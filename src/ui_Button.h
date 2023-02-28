#pragma once
#include "ui_Element.h"
#include "SFML/Graphics.hpp"

enum BUTTON_SIZE {
    TINY,
    SMALL,
    MEDIUM,
    LARGE,
    MASSIVE
};

class Button : public uiElement {
public:
    Button(sf::Vector2f position, sf::Vector2f size, sf::Color color, std::string text, float characterSize);
    Button(sf::Vector2f position, BUTTON_SIZE size, sf::Color color, std::string text);
    ~Button();
    bool m_mutex = false;
    void update(sf::Vector2f mousePos);
    void render(sf::RenderWindow& window);
    bool isClicked();

private:
    sf::RectangleShape m_shape;
    sf::Text m_text;
    sf::Font m_font;
    bool m_clicked;

};

Button::Button(sf::Vector2f position, sf::Vector2f size, sf::Color color, std::string text, float characterSize)
    : m_clicked(false)
{
    if (!m_font.loadFromFile("arial.ttf")) {
        //blegh
    }

    m_shape.setPosition(position);
    m_shape.setSize(size);
    m_shape.setFillColor(color);

    m_text.setString(text);
    m_text.setCharacterSize(characterSize);
    m_text.setFillColor(sf::Color::Black);
    m_text.setFont(m_font);

    //centering math bullshit
    m_text.setPosition(
        m_shape.getPosition().x + (m_shape.getSize().x / 2.0f) - (m_text.getGlobalBounds().width / 2.0f),
        m_shape.getPosition().y + (m_shape.getSize().y / 2.0f) - (m_text.getGlobalBounds().height / 2.0f) - (m_shape.getSize().y / 10)
    );
}

Button::Button(sf::Vector2f position, BUTTON_SIZE size, sf::Color color, std::string text)
    : m_clicked(false)
{
    if (!m_font.loadFromFile("arial.ttf")) {
        //blegh
    }

    m_shape.setPosition(position);
    m_shape.setFillColor(color);

    m_text.setString(text);
    m_text.setFillColor(sf::Color::Black);
    m_text.setFont(m_font);

    sf::Vector2f m_size;
    int m_csize = 14;
    switch (size) {
    case TINY:
        m_size = { 50, 20 };
        m_csize = 10;
        break;
    case SMALL:
        m_size = { 75, 30 };
        m_csize = 12;
        break;
    case MEDIUM:
        m_size = { 100, 40 };
        m_csize = 16;
        break;
    case LARGE:
        m_size = { 150, 60 };
        m_csize = 26;
        break;
    case MASSIVE:
        m_size = { 200, 80 };
        m_csize = 30;
        break;

    default:
        m_size = { 100, 40 };
        m_csize = 14;
        break;
    }

    m_shape.setSize(m_size);
    m_text.setCharacterSize(m_csize);

    //centering math bullshit
    m_text.setPosition(
        m_shape.getPosition().x + (m_shape.getSize().x / 2.0f) - (m_text.getGlobalBounds().width / 2.0f),
        m_shape.getPosition().y + (m_shape.getSize().y / 2.0f) - (m_text.getGlobalBounds().height / 2.0f) - (m_shape.getSize().y / 10)
    );
}

Button::~Button() {
}

void Button::update(sf::Vector2f mousePos) {
    if (m_shape.getGlobalBounds().contains(mousePos)) {
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            m_clicked = true;
        }
    }
}

void Button::render(sf::RenderWindow& window) {
    window.draw(m_shape);
    window.draw(m_text);
}

bool Button::isClicked() {
    if (m_clicked) {
        m_clicked = false;
        return true;
    }
    return false;
}


namespace UIButtonHelper {
    std::vector<Button*> list;

    int createButton(sf::Vector2f position, BUTTON_SIZE size, sf::Color color, std::string text) {
        auto tempEnt = new Button(position, size, color, text);
        tempEnt->id = list.size();

        list.push_back(tempEnt);
        UIElementHelper::list.push_back(tempEnt);

        return tempEnt->id;
    }

    int createExactButton(sf::Vector2f position, sf::Vector2f size, sf::Color color, std::string text, float characterSize) {
        auto tempEnt = new Button(position, size, color, text, characterSize);
        tempEnt->id = list.size();

        list.push_back(tempEnt);
        UIElementHelper::list.push_back(tempEnt);

        return tempEnt->id;
    }
}