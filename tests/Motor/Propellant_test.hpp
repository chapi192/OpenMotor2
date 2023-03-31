#pragma once
#include "TestUtil.hpp"
#include "Motor/Propellant.hpp"
using namespace motor;
#include <iostream>

class PropellantTest {
public:
	PropellantTest() {
		std::cout << "PropellantTest()\n";
		basicCombustionProperties();
		pressureSorting();
	}

	void basicCombustionProperties() {
		Propellant propellant;
		Propellant::Properties prop{ 1, 5, 2, 3, 4, 6, 7 };
		propellant.addProperties(prop);

		auto atLessPressure = propellant.getCombustionProperties(0);
		auto atAccuratePressure = propellant.getCombustionProperties(4);
		auto atMorePressure = propellant.getCombustionProperties(12);

		REQUIRE_EQ(atLessPressure.burnExp, prop.burnExp);  // just arbitrary checks for equality
		REQUIRE_EQ(atLessPressure.combustTemp, prop.combustTemp);
		REQUIRE_EQ(atAccuratePressure.burnCoeff, prop.burnCoeff);
		REQUIRE_EQ(atMorePressure.exhaustMolarMass, prop.exhaustMolarMass);
	}

	void pressureSorting() {
		Propellant propellant;
		Propellant::Properties propLeast = { 1, 4, 1, 0, 0, 0, 0 };
		Propellant::Properties propMid = { 4, 7, 2, 0, 0, 0, 0 };
		Propellant::Properties propMost = { 5, 9, 3, 0, 0, 0, 0 };

		propellant.addProperties(propMost);
		propellant.addProperties(propLeast);
		propellant.addProperties(propMid);

		auto atLessPressure  = propellant.getCombustionProperties(0);
		auto atLeastPressure = propellant.getCombustionProperties(2);
		auto atMidPressure   = propellant.getCombustionProperties(5);
		auto atMostPressure  = propellant.getCombustionProperties(8);
		auto atMorePressure  = propellant.getCombustionProperties(10);

		REQUIRE_EQ(atLessPressure.burnCoeff,  propLeast.burnCoeff);
		REQUIRE_EQ(atLeastPressure.burnCoeff, propLeast.burnCoeff);
		REQUIRE_EQ(atMidPressure.burnCoeff,   propMid.burnCoeff);
		REQUIRE_EQ(atMostPressure.burnCoeff,  propMost.burnCoeff);
		REQUIRE_EQ(atMorePressure.burnCoeff,  propMost.burnCoeff);
	}
};
