#include "TextInputController.hpp"

void TextInputController::addTextInput(
		const tgui::Layout2d& pos,
		const tgui::Layout2d& size,
		const tgui::String& text
) {
	auto textArea = tgui::TextArea::create();
	textArea->addText(text);
	textArea->setPosition(pos);
	textArea->setSize(size);
	textInputs.push_back(textArea);
}