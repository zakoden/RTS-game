#include "grid_function.h"

#include <cassert>

#include <queue>

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

inline uint32_t grid_function::SquaredDistance(const Point& a, const Point& b) {
	return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

inline float grid_function::Distance(const Point& a, const Point& b) {
	return sqrtf(static_cast<float>(SquaredDistance(a, b)));
}

void grid_function::Dijkstra(Grid<float>* distance_ptr, 
	Grid<int>* cluster_ptr) {
	assert(!distance_ptr -> empty());
	Grid<float>& distance = *distance_ptr;

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
		if (std::abs(-pair.first - distance[current]) > EPS)  // -pair.first != distance[y][x]
			continue;

		// Going through all neighboring points
		for (Point point : GetNeighbors(current, distance.size(), distance[0].size())) {
			float dist = Distance(point, current);
			if (distance[point] > distance[current] + dist) {
				distance[point] = distance[current] + dist;
				if (cluster_ptr != nullptr)
					cluster_ptr -> operator[](point) = cluster_ptr -> operator[](current);
				dijkstra.push({ -distance[point], {point.x, point.y} });
			}
		}
	}
}

Grid<Point> grid_function::ProcessBFS(const Grid<bool>& allowed_points, const Point& start) {
	Grid<Point> ancestor(allowed_points.size(), allowed_points[0].size());
	for (uint32_t i = 0; i < allowed_points.size(); ++i)
		for (uint32_t j = 0; j < allowed_points[i].size(); ++j)
			ancestor[i][j] = { j, i };
	std::queue<Point> bfs;
	bfs.push(start);
	while (!bfs.empty()) {
		Point cur = bfs.front();
		bfs.pop();
		for (Point point : GetNeighbors(cur, allowed_points.size(), allowed_points[0].size())) {
			if (ancestor[point] == point && allowed_points[point]) {
				bfs.push(point);
				ancestor[point] = cur;
			}
		}
	}
	return ancestor;
}

vector<Point> grid_function::GetPath(const Grid<bool>& allowed_points, const Point& a, const Point& b) {
	Grid<Point> ancestor = ProcessBFS(allowed_points, a);
	vector<Point> result = { b };
	while (result.back() != a)
		result.push_back(ancestor[result.back()]);
	reverse(result.begin(), result.end());
	return result;
}

vector<Point> grid_function::FindClosest(const Point& start, const Grid<bool>& allowed_points,
	const Grid<bool>& end_points, float min_distance) {
	Grid<Point> ancestor = FromFunction<Point>(allowed_points.size(), allowed_points[0].size(),
		[&](uint32_t i, uint32_t j) {return Point{j, i}; });

	float max_distance = static_cast<float>(allowed_points.size() + allowed_points[0].size());
	Grid<float> distance(allowed_points.size(), allowed_points[0].size(), max_distance);
	distance[start] = 0;

	std::priority_queue<std::pair<float, std::pair<uint32_t, uint32_t>>> dijkstra;
	const float EPS = 1e-3f;
	dijkstra.push({ 0, {start.x, start.y} });

	vector<Point> result;
	while (!dijkstra.empty()) {
		auto pair = dijkstra.top();
		dijkstra.pop();
		uint32_t x = pair.second.first, y = pair.second.second;
		Point current = { x, y };
		if (std::abs(-pair.first - distance[current]) > EPS)  // -pair.first != distance[y][x]
			continue;
		if (end_points[current] && distance[current] + EPS > min_distance) {
			result.push_back(current);
			break;
		}

		// Going through all neighboring points
		for (Point point : GetNeighbors(current, distance.size(), distance[0].size())) {
			if (!allowed_points[point])
				continue;
			float dist = Distance(point, current);
			if (distance[point] > distance[current] + dist) {
				distance[point] = distance[current] + dist;
				ancestor[point] = current;
				dijkstra.push({ -distance[point], {point.x, point.y} });
			}
		}
	}

	if (result.empty())
		return result;
	while (result.back() != start)
		result.push_back(ancestor[result.back()]);
	reverse(result.begin(), result.end());
	return result;
}