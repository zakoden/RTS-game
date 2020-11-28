#pragma once

#include <vector>

#include "block_type.h"
#include "grid.h"
#include "grid_neighbors.h"

using std::vector;

namespace grid_function {
	// Builds grid from function, grid[i][j] = f(i, j)
	template<typename T, class BinaryFunction>
	Grid<T> FromFunction(size_t height, size_t width, BinaryFunction f);

	// Gets square of a distance between two points
	inline uint32_t SquaredDistance(const MapPoint& a, const MapPoint& b);

	// Gets an actual distance between two points
	float Distance(const MapPoint& a, const MapPoint& b);

	// Makes map more smooth
	// For each cell, if 5 neighbors have the same block type, transform that cell into that block type
	void SmoothMap(const GridNeighbors& neighbors, Grid<BlockType>& blocks);

	// Removes small areas
	// Unites small areas with the closest non-small ones
	void RemoveSmallAreas(const GridNeighbors& neighbors, Grid<BlockType>& blocks);

	/* Finds the closest point from start if we:
	1. Can only travel via allowed points
	2. Must end at end point
	3. Have distance at least min_distance
	*/
	vector<MapPoint> FindClosest(const GridNeighbors& neighbors, const MapPoint& start,
		const Grid<char>& allowed_points, const Grid<char>& end_points, float max_distance = 0);

	/* Finds the farthest point from start if we:
	1. Can only travel via allowed points
	2. Must end at end point
	*/
	vector<MapPoint> FindFarthest(const GridNeighbors& neighbors, const MapPoint& start,
		const Grid<char>& allowed_points, const Grid<char>& end_points);
};

template<typename T, class BinaryFunction>
Grid<T> grid_function::FromFunction(size_t height, size_t width, BinaryFunction f) {
	Grid<T> result(height, width);
	for (uint32_t i = 0; i < height; ++i)
		for (uint32_t j = 0; j < width; ++j)
			result[i][j] = f(i, j);
	return result;
}