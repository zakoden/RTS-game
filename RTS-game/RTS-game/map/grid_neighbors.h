#pragma once

#include "grid.h"

#include "grid_function.h"

class GridNeighbors : public Grid<vector<Point>> {
public:
	GridNeighbors(size_t height, size_t width);
};