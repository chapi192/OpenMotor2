#include "TextInputController.hpp"

#include <iostream>
void callback_onTextChange(tgui::TextArea::Ptr textArea) {
	std::cout << "text was changed to: \n"
		<< textArea->getText() << '\n';
}

void TextInputController::addTextInput(
		const tgui::Layout2d& pos,
		const tgui::Layout2d& size,
		const tgui::String& text
) {
	auto textArea = tgui::TextArea::create();
	textArea->addText(text);
	textArea->setPosition(pos);
	textArea->setSize(size);

	textArea->onTextChange(&callback_onTextChange, textArea);

	textInputs.push_back(textArea);
}