#pragma once
#include "SFML/Graphics.hpp"
#include "Utils.h"

class uiElement {
public:
	// list of drawables that define how the ui element looks
	std::vector<sf::Drawable*> drawableList = {};

	//unique id, maybe implement this so we can access / delete elements by id
	int id;

	sf::Vector2f position;
	sf::Vector2f size;

	uiElement() {
		position = { 0,0 };
		size = { 0,0 };
	}

};

namespace UIElementHelper {
	std::vector<uiElement*> list;

	int createUIElement() {
		auto tempEnt = new uiElement();
		tempEnt->id = list.size();

		list.push_back(tempEnt);

		return tempEnt->id;
	}
}