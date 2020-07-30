#pragma once

#include <cstdint>

struct Point {
	uint32_t x, y;

	bool operator==(const Point& other);
	bool operator!=(const Point& other);
};