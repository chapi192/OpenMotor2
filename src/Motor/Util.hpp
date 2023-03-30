#pragma once

extern "C" {
#	define PI 3.141592653589793238462643
	namespace geo {
	inline float circleArea(float diameter) {
		return diameter * diameter / 4 * PI;
	}
	inline float circlePerimeter(float diameter) {
		return diameter * PI;
	}
	inline float cylinderVolume(float diameter, float length) {
		return length * circleArea(diameter);
	}
	inline float radians(float degrees) {
		return degrees * PI / 180;
	}
	}
}
