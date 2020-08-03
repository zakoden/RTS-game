#include "grid_neighbors.h"

GridNeighbors::GridNeighbors(size_t height, size_t width) {
	resize(height, vector<vector<Point>>(width));
	for (uint32_t i = 0; i < height; ++i)
		for (uint32_t j = 0; j < width; ++j)
			operator[](i)[j] = grid_function::GetNeighbors({ j, i }, height, width);
}