#pragma once
#include <float.h>
#include <cmath>

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

/**
 * Solves for the first argument of a function.
 * @warning Args and Params should be of the same types,
 *   just with Args shifted left 1 to account for the missing first argument.
 * @tparam ...Args    types of the arguments
 * @tparam ...Params  types of the function parameters
 * @param result      result of the funciton
 * @param approx0     initial guess (doesn't have to be accurate at all)
 * @param approx1     another guess
 * @param fn          function to solve
 * @param args        constant arguments of fn, 2nd argument and on
 * @return            An approximate solution to the first argument.
 */
template<typename... Params, typename... Args>
float fSolve_NewtonRaphson(
		const float result,
		float approx0, float approx1,
		float fn(Params...), const Args... args
) {
	static_assert(sizeof...(Args) == sizeof...(Params) - 1, "Args size must be one less than Params");
	assert(approx0 != approx1 && "Must define unique approximates.");
	float& x0 = approx0;
	float& x1 = approx1;
	float res0 = fn(x0, args...);
	float res1;
	int i = 0;  // loop count to prevent infinite loops
	while (i != 100 && std::abs(res0 - result) > FLT_EPSILON) {
		i++;
		res1 = fn(x1, args...);
		while (!std::isnormal(res1)) {
			x1 = (x0 + x1) / 2;
			res1 = fn(x1, args...);
		}

		float temp = x1;
		x1 = x0 - (res0 - result) * (x1 - x0) / (res1 - res0);
		x0 = temp;
		res0 = res1;
	}
	return x0;
}

template<typename T>
T calcSum(const std::vector<T>& vec) {
	T sum{};
	for (const T& v : vec)
		sum += v;
	return sum;
}

template<typename T>
T calcMax(const std::vector<T>& vec) {
	T max{ vec[0] };
	for (const T& v : vec)
		if (max < v)
			max = v;
	return max;
}
