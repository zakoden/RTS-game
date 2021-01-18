#pragma once

#include "grid.h"

class GridNeighbors : public Grid<vector<MapPoint>> {
public:
	GridNeighbors(size_t height, size_t width);

	template<typename T>
	GridNeighbors(const Grid<T>& other);
};

template<typename T>
GridNeighbors::GridNeighbors(const Grid<T>& other) {
	if (!other.empty()) {
		*this = GridNeighbors(other.size(), other[0].size());
	}
}
