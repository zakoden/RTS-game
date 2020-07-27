#pragma once

#include <algorithm>
#include <vector>

using std::vector;

namespace grid_function {
	struct Point { uint32_t x, y; };

	// Checks if two points are equal or not
	bool operator==(const Point& a, const Point& b);

	// Gets corner-wise neighbors of a point in a grid with dimensions height x width
	vector<Point> GetNeighbors(const Point& p, size_t height, size_t width);

	// Gets square of a distance between two points
	uint32_t SquaredDistance(const Point& a, const Point& b);

	// Gets an actual distance between two points
	float Distance(const Point& a, const Point& b);

	/* Performs the following:
	1. All points with distance < 1e-3 are considered start points.
	2. Then, by using dijkstra algorithm, it converts arrays such that:
	distance[i][j] = distance to the start point
	cluster[i][j] = number of a cluster of the closest start point (optional)
	*/
	void Dijkstra(vector<vector<float>>* distance, vector<vector<int>>* cluster);

	// Calculates area for each cluster, returns vector with <max number of a cluster> elements
	// vector<uint32_t> CalculateArea(const vector<vector<int>>& cluster);
};