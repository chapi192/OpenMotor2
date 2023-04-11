#pragma once

#include <TGUI/Core.hpp>
#include <TGUI/Widgets/EditBox.hpp>
#include <vector>
#include <string>

class TextController 
{
	public:
		void addText(
			const tgui::Layout2d& position, 
			const tgui::String& text, 
			const tgui::String& name, 
			tgui::Container::Ptr& container, 
			int fontSize = 12,
			std::vector<tgui::TextStyle> textStyle = { tgui::TextStyle::Regular },
			tgui::Color backgroundColor = { tgui::Color::Transparent}, 
			tgui::Color textColor = { tgui::Color::Black }
		);

	private:
		std::vector<tgui::EditBox::Ptr> m_texts;
};
