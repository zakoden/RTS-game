#include "grid_function.h"

#include <cassert>

#include <queue>

inline uint32_t grid_function::SquaredDistance(const Point& a, const Point& b) {
	return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

inline float grid_function::Distance(const Point& a, const Point& b) {
	return sqrtf(static_cast<float>(SquaredDistance(a, b)));
}

void grid_function::Dijkstra(const GridNeighbors& neighbors, 
	Grid<float>* distance_ptr, Grid<int>* cluster_ptr) {
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
		for (Point point : neighbors[current]) {
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

Grid<uint32_t> grid_function::GetAreas(const GridNeighbors& neighbors, 
	const Grid<BlockType>& clusters) {
	size_t height = clusters.size(), width = clusters[0].size();
	Grid<char> visited(height, width, false);
	Grid<uint32_t> result(height, width);
	for (uint32_t i = 0; i < height; ++i) {
		for (uint32_t j = 0; j < width; ++j) {
			if (!visited[i][j]) {
				vector<Point> bfs;
				bfs.push_back({ j, i });
				for (size_t i = 0; i < bfs.size(); ++i) {
					Point current = bfs[i];
					for (Point p : neighbors[current]) {
						if (clusters[p] == clusters[current] && !visited[p]) {
							bfs.push_back(p);
							visited[p] = true;
						}
					}
				}

				for (Point point : bfs)
					result[point] = static_cast<uint32_t>(bfs.size());
			}
		}
	}
	return result;
}

vector<Point> grid_function::FindClosest(const GridNeighbors& neighbors, const Point& start,
	const Grid<char>& allowed_points, const Grid<char>& end_points, float min_distance) {
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
		for (Point point : neighbors[current]) {
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
