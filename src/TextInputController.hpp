#pragma once

#include <TGUI/Core.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Widgets/TextArea.hpp>

#include <vector>

class TextInputController {
public:
	std::vector<tgui::TextArea::Ptr> textInputs;

	void addTextInput(
			const tgui::Layout2d& pos,
			const tgui::Layout2d& size,
			const tgui::String& text = ""
	);
};