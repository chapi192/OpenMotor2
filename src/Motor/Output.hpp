#pragma once
#include "SimData.hpp"
#include "Util.hpp"
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Widgets/Label.hpp>
#include <TGUI/Widgets/ListView.hpp>

#include <map>

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

		addItem("Impulse"            , "N*s"       );
		addItem("ISP"                , "s"         );
		addItem("Burn Time"          , "s"         );
		addItem("Average Thrust"     , "N"         );
		addItem("Peak Thrust"        , "N"         );
		addItem("Volume Loading"     , "%"         );
		addItem("Average Pressure"   , "Pa"        );
		addItem("Peak Pressure"      , "Pa"        );
		addItem("Initial KN"         , ""          );
		addItem("Peak KN"            , ""          );
		addItem("Ideal Thrust Coeff" , ""          );
		addItem("Propellant Mass"    , "kg"        );
		addItem("Propellant Length"  , "m"         );
		addItem("Port Throat Ratio"  , ""          );
		addItem("Peak Mass Flux"     , "kg/(s*m^2)");
		addItem("AdjustedThrustCoeff", ""          );
	}

	void update(const SimData& simData) {
		setItemValue("Burn Time", simData.m_time.back());
		setItemValue("Peak Thrust", calcMax(simData.m_force));
		setItemValue("Volume Loading", simData.m_volumeLoading[0]);
		setItemValue("Initial KN", simData.m_kn[0]);
		setItemValue("Peak KN", calcMax(simData.m_kn));

		const float accGravity = 9.80665;

		float sumForce = calcSum(simData.m_force);
		setItemValue("Average Thrust", sumForce / simData.m_force.size());

		float dt = (simData.m_time.back() - simData.m_time[0]) / (simData.m_time.size() - 1);
		float impulse = sumForce * dt;
		setItemValue("Impulse", impulse);

		float propMass = 0;
		for (const auto& grain : simData.m_grains) {
			propMass += grain.m_mass[0];
		}
		float isp = impulse / (propMass * accGravity);
		setItemValue("ISP", isp);

		setItemValue("Peak Pressure", calcMax(simData.m_pressure));
		setItemValue("Average Pressure", calcSum(simData.m_pressure) / simData.m_pressure.size());
	}
private:
	inline void addItem(const std::string& label, const std::string& unit) {
		labelMap[label] = dataList->addItem({ label, "", unit });
	}

	inline void setItemValue(const std::string& label, float value) {
		assert(labelMap.find(label) != labelMap.end() && "Invalid label");

		dataList->changeSubItem(labelMap[label], 1, tgui::String::fromNumber(value));
	}
private:
	bool isMouseOnWidget(tgui::Vector2f pos) const override {
		return tgui::FloatRect{ getPosition().x, getPosition().y, getSize().x, getSize().y }.contains(pos);
	}

	Widget::Ptr clone() const override {
		return std::make_shared<Output>(*this);
	}
private:
	tgui::Label::Ptr label;
	tgui::ListView::Ptr dataList;

	std::unordered_map<std::string, size_t> labelMap;
};
}
