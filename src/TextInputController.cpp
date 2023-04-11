#include "TextInputController.hpp"

#include <iostream>
void callback_onTextChange(tgui::EditBox::Ptr editBox) {
	std::cout << "text was changed to: \n"
		<< editBox->getText() << '\n';
}

void TextInputController::addTextInput(
		const tgui::Layout2d& pos,
		const tgui::Layout2d& size,
		const tgui::String& text
) {
	auto editBox = tgui::EditBox::create();
	editBox->setText(text);
	editBox->setPosition(pos);
	editBox->setSize(size);

	editBox->onTextChange(&callback_onTextChange, editBox);

	textInputs.push_back(editBox);
}

void TextInputController::addTextInput(
	const tgui::Layout2d& positionInContainer, 
	const tgui::Layout2d& size, 
	const tgui::String name, 
	tgui::Container::Ptr& container, 
	int maxChars, 
	bool readOnly, 
	std::string txt
) {
	auto editBox = tgui::EditBox::create();
	editBox->setMaximumCharacters(maxChars);
	editBox->setText(txt);
	editBox->setPosition(positionInContainer);
	editBox->setSize(size);
	editBox->setReadOnly(readOnly);

	editBox->onTextChange(&callback_onTextChange, editBox);

	container->add(editBox, name);

	textInputs.emplace_back(editBox);
}

void TextInputController::addTextInput(
	const tgui::Layout2d& positionInGUI, 
	const tgui::Layout2d& size, 
	const tgui::String name, 
	tgui::BackendGui& gui, 
	int maxChars, 
	bool readOnly, 
	std::string txt
) {
	auto editBox = tgui::EditBox::create();

	editBox->setMaximumCharacters(maxChars);
	editBox->setText(txt);
	editBox->setPosition(positionInGUI);
	editBox->setSize(size);
	editBox->setReadOnly(readOnly);

	editBox->onTextChange(&callback_onTextChange, editBox);

	gui.add(editBox, name);

	textInputs.emplace_back(editBox);
}

std::vector<tgui::EditBox::Ptr> TextInputController::getTextInputs() { return textInputs; }