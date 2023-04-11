#pragma once

#include <TGUI/Core.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Widgets/EditBox.hpp>

#include <vector>

class TextInputController {
public:

	void addTextInput(
			const tgui::Layout2d& pos,
			const tgui::Layout2d& size,
			const tgui::String& text = ""
	);

	void addTextInput(
		const tgui::Layout2d& positionInContainer, 
		const tgui::Layout2d& size, 
		const tgui::String name, 
		tgui::Container::Ptr& container, 
		int maxChars = 15,
		bool readOnly = false, 
		std::string txt = ""
	);
	
	void addTextInput(
		const tgui::Layout2d& positionInGUI, 
		const tgui::Layout2d& size, 
		const tgui::String name,
		tgui::BackendGui& gui, 
		int maxChars = 15,
		bool readOnly = false, 
		std::string txt = ""
	);

	std::vector<tgui::EditBox::Ptr> getTextInputs();

private:
	std::vector<tgui::EditBox::Ptr> textInputs;
};