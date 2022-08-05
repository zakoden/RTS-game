#include "grid_function.h"

#include <cassert>

#include <iostream>
#include <math.h>
#include <queue>

#include "../triple.h"

inline uint32_t grid_function::SquaredDistance(const MapPoint& a, const MapPoint& b) {
	return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

inline float grid_function::Distance(const MapPoint& a, const MapPoint& b) {
	return sqrtf(static_cast<float>(SquaredDistance(a, b)));
}

void grid_function::SmoothMap(const GridNeighbors& neighbors, Grid<BlockType>& blocks) {
	uint32_t transformed_cells = 0;
	for (uint32_t i = 0; i < blocks.size(); ++i) {
		for (uint32_t j = 0; j < blocks[i].size(); ++j) {
			int block = -1; int count = 0;
			for (MapPoint point : neighbors[i][j]) {
				if (blocks[point] == block) {
					++count;
				}
				else {
					--count;
					if (count < 0) {
						count = 1;
						block = blocks[point];
					}
				}
			}

			if (blocks[i][j] != block) {
				count = 0;
				for (MapPoint point : neighbors[i][j])
					count += blocks[point] == block;
				if (count >= 5) {
					++transformed_cells;
					blocks[i][j] = static_cast<BlockType>(block);
				}
			}
		}
	}
	std::cerr << "SmoothMap: Transformed " << transformed_cells << " cells" << std::endl;
}

void grid_function::RemoveSmallAreas(const GridNeighbors& neighbors, Grid<BlockType>& blocks, uint32_t area_min) {
	size_t height = blocks.size(), width = blocks[0].size();

	// 1. Get areas for each cell
	Grid<uint32_t> areas(height, width, 0);
	for (uint32_t i = 0; i < height; ++i) {
		for (uint32_t j = 0; j < width; ++j) {
			if (areas[i][j] == 0) {
				vector<MapPoint> bfs = { {j, i} };
				areas[i][j] = 1;
				for (size_t i = 0; i < bfs.size(); ++i) {
					MapPoint current = bfs[i];
					for (MapPoint p : neighbors[current]) {
						if (blocks[p] == blocks[current] && areas[p] == 0) {
							bfs.push_back(p);
							areas[p] = 1;
						}
					}
				}

				for (MapPoint point : bfs)
					areas[point] = static_cast<uint32_t>(bfs.size());
			}
		}
	}

	// 2. For each point find the closest with allowed area
	uint32_t transformed_cells = 0;
	std::queue<MapPoint> bfs;
	for (uint32_t i = 0; i < height; ++i)
		for (uint32_t j = 0; j < width; ++j)
			if (areas[i][j] >= area_min)
				bfs.push({ j, i });
	while (!bfs.empty()) {
		MapPoint cur = bfs.front();
		bfs.pop();
		for (MapPoint point : neighbors[cur]) {
			if (areas[point] < area_min) {
				areas[point] = areas[cur];
				blocks[point] = blocks[cur];
				++transformed_cells;
				bfs.push(point);
			}
		}
	}

	std::cerr << "RemoveSmallAreas: Transformed " << transformed_cells << " cells" << std::endl;
}

vector<MapPoint> grid_function::FindClosest(const GridNeighbors& neighbors, const MapPoint& start,
	const Grid<char>& allowed_points, const Grid<char>& end_points, float max_distance) {
	Grid<MapPoint> ancestor(allowed_points.size(), allowed_points[0].size());
	ancestor[start] = start;

	float max_pos_distance = static_cast<float>(allowed_points.size() + allowed_points[0].size());
	Grid<float> distance(allowed_points.size(), allowed_points[0].size(), max_pos_distance);
	distance[start] = 0;

	std::priority_queue<Triple> dijkstra;
	dijkstra.push({ 0, start });

	vector<MapPoint> result;
	while (!dijkstra.empty()) {
		Triple pair = dijkstra.top();
		dijkstra.pop();
		const MapPoint& current = pair.point;
		if (pair.distance > distance[current] || Distance(current, start) > max_distance)
			continue;
		if (end_points[current]) {
			result.push_back(current);
			//std::cerr << distance[current] << std::endl;
			break;
		}

		// Going through all neighboring points
		for (MapPoint point : neighbors[current]) {
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
	return result;
}

vector<MapPoint> grid_function::FindFarthest(const GridNeighbors& neighbors, const MapPoint& start,
	const Grid<char>& allowed_points, const Grid<char>& end_points) {
	Grid<MapPoint> ancestor(allowed_points.size(), allowed_points[0].size());
	ancestor[start] = start;

	float max_distance = static_cast<float>(allowed_points.size() + allowed_points[0].size());
	Grid<float> distance(allowed_points.size(), allowed_points[0].size(), max_distance);
	distance[start] = 0;

	std::priority_queue<Triple> dijkstra;
	dijkstra.push({ 0, start });

	vector<MapPoint> result;
	while (!dijkstra.empty()) {
		Triple pair = dijkstra.top();
		dijkstra.pop();
		const MapPoint& current = pair.point;
		if (pair.distance > distance[current])
			continue;
		if (end_points[current])
			result = { current };

		// Going through all neighboring points
		for (MapPoint point : neighbors[current]) {
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
	return result;
}