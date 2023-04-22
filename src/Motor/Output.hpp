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
		label->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
		label->setSize({"40%", label->getSizeLayout().y});
		add(label);

		dataList = tgui::ListView::create();
		add(dataList);
		dataList->setPosition(0, bindBottom(label));
		dataList->setSize({label->getSizeLayout().x, "100%" - dataList->getPositionLayout().y});
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
		const float accGravity = 9.80665;

		float maxForce = 0;
		float sumForce = 0;
		for (float force : simData.m_force) {
			sumForce += force;
			if (maxForce < force)
				maxForce = force;
		}
		float avgForce = sumForce / simData.m_force.size();

		float dt = (simData.m_time.back() - simData.m_time[0]) / (simData.m_time.size() - 1);
		float impulse = sumForce * dt;

		float propMass = 0;
		for (const auto& grain : simData.m_grains) {
			propMass += grain.m_mass[0];
		}
		float isp = impulse / (propMass * accGravity);

		// function turning float into a tgui::String
		auto toStr = tgui::String::fromNumber<float>;
		dataList->changeSubItem(0, 1, toStr(impulse));
		dataList->changeSubItem(1, 1, toStr(isp));
		dataList->changeSubItem(2, 1, toStr(simData.m_time.back()));
		dataList->changeSubItem(3, 1, toStr(avgForce));
		dataList->changeSubItem(4, 1, toStr(maxForce));
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
