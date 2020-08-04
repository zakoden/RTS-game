#include "grid_function.h"

#include <cassert>

#include <iostream>
#include <queue>

inline uint32_t grid_function::SquaredDistance(const Point& a, const Point& b) {
	return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

inline float grid_function::Distance(const Point& a, const Point& b) {
	return sqrtf(static_cast<float>(SquaredDistance(a, b)));
}


struct triple {
	float distance; Point point;
	bool operator<(const triple& other) const { return distance > other.distance; }
};

void grid_function::Dijkstra(const GridNeighbors& neighbors, 
	Grid<float>* distance_ptr, Grid<int>* cluster_ptr) {
	assert(!distance_ptr -> empty());
	Grid<float>& distance = *distance_ptr;

	size_t height = distance.size(), width = distance[0].size();

	// Elements in dijkstra are in form {-distance, {x, y} (point)}
	std::priority_queue<triple> dijkstra;
	const float EPS = 1e-3f;

	// Inititalizing priority queue
	for (uint32_t i = 0; i < height; ++i)
		for (uint32_t j = 0; j < width; ++j)
			if (distance[i][j] < EPS)
				dijkstra.push({ 0, {j, i} });

	while (!dijkstra.empty()) {
		triple pair = dijkstra.top();
		dijkstra.pop();
		const Point& current = pair.point;
		if (pair.distance > distance[current])
			continue;

		// Going through all neighboring points
		for (Point point : neighbors[current]) {
			float dist = Distance(point, current);
			if (distance[point] > distance[current] + dist) {
				distance[point] = distance[current] + dist;
				if (cluster_ptr != nullptr)
					cluster_ptr -> operator[](point) = cluster_ptr -> operator[](current);
				dijkstra.push({ distance[point], point });
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
	Grid<Point> ancestor(allowed_points.size(), allowed_points[0].size());
	ancestor[start] = start;

	float max_distance = static_cast<float>(allowed_points.size() + allowed_points[0].size());
	Grid<float> distance(allowed_points.size(), allowed_points[0].size(), max_distance);
	distance[start] = 0;

	std::priority_queue<triple> dijkstra;
	const float EPS = 1e-3f;
	dijkstra.push({ 0, start });

	vector<Point> result;
	while (!dijkstra.empty()) {
		triple pair = dijkstra.top();
		dijkstra.pop();
		const Point& current = pair.point;
		if (pair.distance > distance[current]) 
			continue;
		if (end_points[current] && distance[current] + EPS > min_distance) {
			result.push_back(current);
			std::cerr << distance[current] << std::endl;
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
				dijkstra.push({ distance[point], point });
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
