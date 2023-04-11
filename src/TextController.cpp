#include "TextController.hpp"

void TextController::addText(
	const tgui::Layout2d& position,
	const tgui::String& text,
	const tgui::String& name,
	tgui::Container::Ptr& container,
	int fontSize,
	std::vector<tgui::TextStyle> textStyle,
	tgui::Color backgroundColor,
	tgui::Color textColor
) {
	auto textBox = tgui::EditBox::create();
	auto renderer = textBox->getRenderer();

	textBox->setPosition(position);
	renderer->setTextColorDisabled(textColor);
	renderer->setBackgroundColorDisabled(backgroundColor);
	renderer->setBorderColorDisabled(backgroundColor);
	
	textBox->setTextSize(fontSize);
	textBox->setReadOnly(true);
	textBox->setEnabled(false);

	for (int idx = 0; idx < textStyle.size(); idx++) {
		textBox->getRenderer()->setTextStyle(textStyle[idx]);
	}

	textBox->setSize(text.length() * 0.75f * fontSize, 1.6f * fontSize);
	textBox->setText(text);

	container->add(textBox, name);

	m_texts.emplace_back(textBox);
}
