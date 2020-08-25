#pragma once

#include "grid.h"

class GridNeighbors : public Grid<vector<MapPoint>> {
public:
	GridNeighbors(size_t height, size_t width);
};