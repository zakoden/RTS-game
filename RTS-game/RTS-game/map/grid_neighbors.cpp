#include "grid_neighbors.h"

GridNeighbors::GridNeighbors(size_t height, size_t width) {
	resize(height, vector<vector<Point>>(width));
	for (uint32_t i = 0; i < height; ++i) {
		for (uint32_t j = 0; j < width; ++j) {
			Point p = { j, i };
			for (int dx = -1; dx <= 1; ++dx) {
				if ((dx == -1 && p.x == 0) || (dx == 1 && p.x == width - 1))  // x is out of bounds
					continue;

				for (int dy = -1; dy <= 1; ++dy) {
					if ((dy == -1 && p.y == 0) || (dy == 1 && p.y == height - 1))  // y is out of bounds
						continue;

					operator[](i)[j].push_back({ p.x + dx, p.y + dy });
				}
			}
		}
	}
}