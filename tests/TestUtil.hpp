#pragma once
#include <iostream>

#define REQUIRE(x) \
	if (!(x)) \
		std::cerr << "False statement: " << #x << "\n\tat " << __FILE__ << ":" << __LINE__ << '\n'; \

#define REQUIRE_EQ(observed, expected) \
	if (!(observed == expected)) \
		std::cerr << "False statement: " << #observed << " == " << #expected \
				<< "\n\tat " << __FILE__ << ":" << __LINE__ \
				<< "\n\t-> " << (observed) << " != " << (expected) << '\n';
