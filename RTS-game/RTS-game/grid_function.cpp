#include "grid_function.h"

#include <cassert>

#include <queue>

using grid_function::Point;

bool grid_function::operator==(const Point& a, const Point& b) {
	return a.x == b.x && a.y == b.y;
}

vector<Point> grid_function::GetNeighbors(const Point& p, size_t height, size_t width) {
	vector<Point> result;
	for (int dx = -1; dx <= 1; ++dx) {
		if ((dx == -1 && p.x == 0) || (dx == 1 && p.x == width - 1))  // x is out of bounds
			continue;

		for (int dy = -1; dy <= 1; ++dy) {
			if ((dy == -1 && p.y == 0) || (dy == 1 && p.y == height - 1))  // y is out of bounds
				continue;

			result.push_back({ p.x + dx, p.y + dy });
		}
	}

	return result;
}

uint32_t grid_function::SquaredDistance(const Point& a, const Point& b) {
	return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

float grid_function::Distance(const Point& a, const Point& b) {
	return sqrtf(static_cast<float>(SquaredDistance(a, b)));
}

void grid_function::Dijkstra(vector<vector<float>>* distance_ptr, 
	vector<vector<int>>* cluster_ptr = nullptr) {
	assert(!distance_ptr -> empty());
	vector<vector<float>>& distance = *distance_ptr;

	// Elements in dijkstra are in form {-distance, {x, y} (point)}
	std::priority_queue<std::pair<float, std::pair<uint32_t, uint32_t>>> dijkstra;
	const float EPS = 1e-3f;

	// Inititalizing priority queue
	for (uint32_t i = 0; i < distance.size(); ++i)
		for (uint32_t j = 0; j < distance[i].size(); ++j)
			if (distance[i][j] < EPS)
				dijkstra.push({ 0, {j, i} });

	while (!dijkstra.empty()) {
		auto pair = dijkstra.top();
		dijkstra.pop();
		uint32_t x = pair.second.first, y = pair.second.second;
		Point current = { x, y };
		if (std::abs(-pair.first - distance[y][x]) > EPS)  // -pair.first != distance[y][x]
			continue;

		// Going through all neighboring points
		for (Point point : GetNeighbors(current, distance.size(), distance[0].size())) {
			uint32_t to_x = point.x, to_y = point.y;
			float dist = Distance(point, current);
			if (distance[to_y][to_x] > distance[y][x] + dist) {
				distance[to_y][to_x] = distance[y][x] + dist;
				if (cluster_ptr != nullptr)
					cluster_ptr -> operator[](to_y)[to_x] = cluster_ptr -> operator[](y)[x];
				dijkstra.push({ -distance[to_y][to_x], {to_x, to_y} });
			}
		}
	}
}

/*
vector<uint32_t> grid_function::CalculateArea(const vector<vector<int>>& cluster) {
	vector<uint32_t> area;
	for (auto& row : cluster)
		area.resize(*std::max_element(row.begin(), row.end()), 0);
	for (size_t i = 0; i < cluster.size(); ++i)
		for (size_t j = 0; j < cluster.size(); ++j)
			++area[cluster[i][j]];
	return area;
}
*/