#pragma once

#include <algorithm>
#include <vector>

#include "grid.h"

using std::vector;

namespace grid_function {
	// Builds grid from function, grid[i][j] = f(i, j)
	template<typename T, class BinaryFunction>
	Grid<T> FromFunction(size_t height, size_t width, BinaryFunction f);

	// Gets corner-wise neighbors of a point in a grid with dimensions height x width
	vector<Point> GetNeighbors(const Point& p, size_t height, size_t width);

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
	void Dijkstra(Grid<float>* distance_ptr, Grid<int>* cluster_ptr = nullptr);

	// Uses 'bfs' on allowed points from `start` point and returns grid,
	// where each cell shows cell from which this cell can be reached
	Grid<Point> ProcessBFS(const Grid<bool>& allowed_points, const Point& start);

	// Finds the shortest path between points `a` and `b` via allowed points and returns it
	vector<Point> GetPath(const Grid<bool>& allowed_points, const Point& a, const Point& b);

	vector<Point> FindClosest(const Point& start, const Grid<bool>& allowed_points,
		const Grid<bool>& end_points, float min_distance = 0);
};


template<typename T, class BinaryFunction>
Grid<T> grid_function::FromFunction(size_t height, size_t width, BinaryFunction f) {
	Grid<T> result(height, width);
	for (uint32_t i = 0; i < height; ++i)
		for (uint32_t j = 0; j < width; ++j)
			result[i][j] = f(i, j);
	return result;
}