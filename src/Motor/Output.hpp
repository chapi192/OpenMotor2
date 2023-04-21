#pragma once
#include "SimData.hpp"
#include <TGUI/Backend/SFML-Graphics.hpp>

namespace motor {
class Output : public tgui::Container {
public:
	typedef std::shared_ptr<Output> Ptr;
	typedef std::shared_ptr<const Output> ConstPtr;

	Output(const tgui::Layout2d& size, const SimData& simData) :
			Container{ "Output", true } {
		m_size = size;
	}

	static Ptr create(const tgui::Layout2d& size, const SimData& simData) {
		return std::make_shared<Output>(size, simData);
	}
private:
	bool isMouseOnWidget(tgui::Vector2f pos) const override {
		return tgui::FloatRect{ getPosition().x, getPosition().y, getSize().x, getSize().y }.contains(pos);
	}

	Widget::Ptr clone() const override {
		return std::make_shared<Output>(*this);
	}
};
}
