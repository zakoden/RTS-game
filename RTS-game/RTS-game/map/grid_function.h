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
	inline uint32_t SquaredDistance(const Point& a, const Point& b);

	// Gets an actual distance between two points
	inline float Distance(const Point& a, const Point& b);

	/* Performs the following:
	1. All points with distance < 1e-3 are considered starting points.
	2. Then, by using dijkstra algorithm, it converts arrays such that:
	distance[i][j] = distance to the starting closest point (SCP)
	cluster[i][j] = number of a cluster of the SCP
	If cluster is nullptr, it is not accessed
	*/
	void Dijkstra(const GridNeighbors& neighbors,
		Grid<float>* distance_ptr, Grid<int>* cluster_ptr = nullptr);

	// For each cell returns the area of a cluster it's located in
	Grid<uint32_t> GetAreas(const GridNeighbors& neighbors,
		const Grid<BlockType>& clusters);

	/* Finds the closest point from start if we:
	1. Can only travel via allowed points
	2. Must end at end point
	3. Have distance at least min_distance
	*/
	vector<Point> FindClosest(const GridNeighbors& neighbors, const Point& start,
		const Grid<char>& allowed_points, const Grid<char>& end_points, float max_distance = 0);

	/* Finds the farthest point from start if we:
	1. Can only travel via allowed points
	2. Must end at end point
	*/
	vector<Point> FindFarthest(const GridNeighbors& neighbors, const Point& start,
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