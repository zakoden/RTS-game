#pragma once

#include <cstdint>

struct MapPoint {
	uint32_t x, y;

	bool operator==(const MapPoint& other);
	bool operator!=(const MapPoint& other);
};