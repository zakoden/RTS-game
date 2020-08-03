#pragma once

#include "grid.h"

class GridNeighbors : public Grid<vector<Point>> {
public:
	GridNeighbors(size_t height, size_t width);
};