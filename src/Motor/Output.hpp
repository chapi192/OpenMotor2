#pragma once
#include "SimData.hpp"
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Widgets/Label.hpp>
#include <TGUI/Widgets/ListView.hpp>

namespace motor {
class Output : public tgui::Container {
public:
	typedef std::shared_ptr<Output> Ptr;
	typedef std::shared_ptr<const Output> ConstPtr;

	Output(const tgui::Layout2d& size, const SimData& simData) :
			Container{ "Output", true } {
		m_size = size;

		label = tgui::Label::create("Motor Statistics");
		add(label);

		dataList = tgui::ListView::create();
		add(dataList);
		dataList->setPosition(bindLeft(label), bindBottom(label));
		dataList->setSize({"40%", "100%" - dataList->getPositionLayout().y});
		dataList->setResizableColumns(true);
		dataList->setHeaderVisible(false);
		dataList->setExpandLastColumn(true);
		initDataList();

		update(simData);
	}

	static Ptr create(const tgui::Layout2d& size, const SimData& simData) {
		return std::make_shared<Output>(size, simData);
	}

	void initDataList() {
		dataList->addColumn("Labels", dataList->getSize().x * 2.0/4);
		dataList->addColumn("Values", dataList->getSize().x * 1.0/4);
		dataList->addColumn("Units");

		dataList->addItem({"Impulse"            , "", "N*s"       });
		dataList->addItem({"ISP"                , "", "s"         });
		dataList->addItem({"Burn Time"          , "", "s"         });
		dataList->addItem({"Average Thrust"     , "", "N"         });
		dataList->addItem({"Peak Thrust"        , "", "N"         });
		dataList->addItem({"Volume Loading"     , "", "%"         });
		dataList->addItem({"Average Pressure"   , "", "Pa"        });
		dataList->addItem({"Peak Pressure"      , "", "Pa"        });
		dataList->addItem({"Initial KN"         , "", ""          });
		dataList->addItem({"Peak KN"            , "", ""          });
		dataList->addItem({"Ideal Thrust Coeff" , "", ""          });
		dataList->addItem({"Propellant Mass"    , "", "kg"        });
		dataList->addItem({"Propellant Length"  , "", "m"         });
		dataList->addItem({"Port Throat Ratio"  , "", ""          });
		dataList->addItem({"Peak Mass Flux"     , "", "kg/(s*m^2)"});
		dataList->addItem({"AdjustedThrustCoeff", "", ""          });
	}

	void update(const SimData& simData) {
	}
private:
	bool isMouseOnWidget(tgui::Vector2f pos) const override {
		return tgui::FloatRect{ getPosition().x, getPosition().y, getSize().x, getSize().y }.contains(pos);
	}

	Widget::Ptr clone() const override {
		return std::make_shared<Output>(*this);
	}
public:
	tgui::Label::Ptr label;
	tgui::ListView::Ptr dataList;
};
}
