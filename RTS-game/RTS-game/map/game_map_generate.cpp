#include "game_map.h"

#include <cassert>
#include <chrono>

#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "perlin.h"
#include "diamond_square.h"
#include "grid_function.h"
#include "grid_neighbors.h"
#include "time_measurer.h"

using grid_function::FromFunction;
using std::vector;

void GameMap::Generate() {
	const std::unordered_set<BlockType> WATER_TYPES = { WATER_SHALLOW, WATER, WATER_DEEP };
	//const float MOUNTAIN_LOW_HEIGHT = 0.19f, MOUNTAIN_HIGH_HEIGHT = 0.25f;
	const float MOUNTAIN_LOW_HEIGHT = 0.70f, MOUNTAIN_HIGH_HEIGHT = 0.85f;
	const float WATER_NORMAL_LEVEL = -0.45f;
	const float WATER_DEEP_LEVEL = -0.80f, WATER_SHALLOW_LEVEL = -0.25f;
	const uint32_t AREA_MIN = 12;

	unsigned int seed = static_cast<unsigned int>(time(0));
	TimeMeasurer time;
	srand(seed);
	std::cerr << "Seed is " << seed << std::endl;

	uint32_t height = GetHeight(), width = GetWidth();

	GridNeighbors neighbors{ height, width };
	// Scatter random points (that wil be centers of clusters)
	const uint32_t CHUNK_SIZE = 500;
	const uint32_t CHUNKS_COUNT = (height * width) / CHUNK_SIZE;

	float max_distance = static_cast<float>(height + width);
	// A suboptimal distance between opposing corners

	// Distance between some point and closest cluster point
	Grid<float> distance(height, width, max_distance);

	// To which cluster this point belongs (-1 for "no cluster")
	Grid<int> cluster(height, width, -1);

	vector<Point> centers(CHUNKS_COUNT);
	for (uint32_t i = 0; i < CHUNKS_COUNT; ++i) {
		uint32_t x = rand() % width, y = rand() % height;  // The random point
		// Repicking the point until it doesn't intersect with others
		while (cluster[y][x] != -1)
			x = rand() % width, y = rand() % height;
		centers[i] = { x, y };
		cluster[y][x] = i;
		distance[y][x] = 0;
	}

	time.PrintTime("Before building voronoi diagram");
	/* 2. Building voronoi diagram
		How? Let's do dijkstra algorithm from all current points,
		then for each cell c we'll find the closest point,
		and c will belong to closest point's cluster
		That will take O(area * log(area)) (area = height_ * width_)
	*/
	grid_function::Dijkstra(neighbors, &distance, &cluster);
	time.PrintTime("Building voronoi diagram");

	// 3. Give to each cluster a random tile
	const vector<BlockType> ALLOWED_BLOCKS = { DESERT, GRASS_LIGHT, GRASS, GRASS_OTHER };
	const vector<BlockType> RARE_BLOCKS = { AMBER, GRASS_PURPLE, DESERT_PURPLE };
	vector<BlockType> cluster_type{ CHUNKS_COUNT };
	for (uint32_t i = 0; i < CHUNKS_COUNT; ++i) {
		if (rand() % 5 == 0)
			cluster_type[i] = RARE_BLOCKS[rand() % RARE_BLOCKS.size()];
		else
			cluster_type[i] = ALLOWED_BLOCKS[rand() % ALLOWED_BLOCKS.size()];
	}
	time.PrintTime("Give to each cluster a random tile");

	// Give the result to blocks grid
	Grid<BlockType> blocks = FromFunction<BlockType>(height, width,
		[&](size_t i, size_t j) { return cluster_type[cluster[i][j]]; });
	time.PrintTime("Give the result to blocks grid");

	// Build water and mountains via perlin noise
	//Grid<float> heights = perlin::GetHeights(height, width);
	Grid<float> heights = diamond_square::GetHeights(height, width);
	const std::unordered_set<BlockType> HEIGHT_TYPES = { MOUNTAIN_HIGH, MOUNTAIN_LOW, WATER_DEEP, WATER, WATER_SHALLOW };
	for (uint32_t i = 0; i < height; ++i) {
		for (uint32_t j = 0; j < width; ++j) {
			//blocks[i][j] = GRASS;
			if (heights[i][j] < WATER_DEEP_LEVEL)
				blocks[i][j] = WATER_DEEP;
			else if (heights[i][j] < WATER_NORMAL_LEVEL)
				blocks[i][j] = WATER;
			else if (heights[i][j] < WATER_SHALLOW_LEVEL)
				blocks[i][j] = WATER_SHALLOW;
			else if (heights[i][j] > MOUNTAIN_HIGH_HEIGHT)
				blocks[i][j] = MOUNTAIN_HIGH;
			else if (heights[i][j] > MOUNTAIN_LOW_HEIGHT)
				blocks[i][j] = MOUNTAIN_LOW;
		}
	}
	time.PrintTime("Build water and mountains via perlin noise");

	
	
	// Unite small areas with the closest non-small ones
	Grid<uint32_t> areas = grid_function::GetAreas(neighbors, blocks);
	Grid<float> distance_to_good_cell = FromFunction<float>(height, width,
		[&](size_t i, size_t j) { return areas[i][j] < AREA_MIN; });

	Grid<int> cluster_as_block_type = FromFunction<int>(height, width,
		[&](size_t i, size_t j) { return blocks[i][j]; });
	grid_function::Dijkstra(neighbors, &distance_to_good_cell, &cluster_as_block_type);
	for (uint32_t i = 0; i < height; ++i)
		for (uint32_t j = 0; j < width; ++j)
			if (areas[i][j] < AREA_MIN)
				blocks[i][j] = static_cast<BlockType>(cluster_as_block_type[i][j]);


	/*
	// Make on_border grid
	Grid<char> on_border(height, width, false);
	{
		for (uint32_t i = 0; i < height; ++i) {
			for (uint32_t j = 0; j < width; ++j) {
				for (Point point : neighbors[i][j]) {
					if (blocks[i][j] != blocks[point] &&
						blocks[i][j] != WATER && blocks[point] != WATER
						&& blocks[i][j] != blocks[point]) {
						on_border[i][j] = true;
						break;
					}
				}
			}
		}

		for (uint32_t i = 0; i < height; ++i) {
			for (uint32_t j = 0; j < width; ++j) {
				for (Point point : neighbors[i][j]) {
					if (blocks[i][j] == WATER && blocks[point] != WATER && on_border[point.y][point.x]) {
						on_border[i][j] = true;
						break;
					}
				}
			}
		}
	}
	time.PrintTime("Make on_border grid"); */
	/*
	areas = grid_function::GetAreas(neighbors, blocks);
	// Add rivers
	{
		// 1. Choose possible river sources
		const uint32_t MIN_DEEP_WATER_AREA = 10;
		std::unordered_set<int> possible_clusters_set;
		for (uint32_t i = 0; i < height; ++i)
			for (uint32_t j = 0; j < width; ++j)
				if (blocks[i][j] == WATER_DEEP && areas[i][j] > MIN_DEEP_WATER_AREA)
					possible_clusters_set.insert(cluster[i][j]);
		vector<int> possible_clusters(possible_clusters_set.begin(), possible_clusters_set.end());

		vector<vector<Point>> border_points(CHUNKS_COUNT);
		for (uint32_t i = 0; i < height; ++i)
			for (uint32_t j = 0; j < width; ++j)
				if (on_border[i][j] && WATER_TYPES.count(blocks[i][j]))
					border_points[cluster[i][j]].push_back({ j, i });

		int total_river_area_left = 3000;
		if (possible_clusters.empty())  // No clusters => no rivers
			total_river_area_left = 0;

		Grid<char> is_water_type = FromFunction<char>(height, width,
			[&](size_t i, size_t j) { return WATER_TYPES.count(blocks[i][j]); });
		Grid<char> all_true(height, width, true);
		Grid<char> is_water = FromFunction<char>(height, width,
			[&](size_t i, size_t j) { return blocks[i][j] == WATER; });
		Grid<char> is_water_shallow = FromFunction<char>(height, width,
			[&](size_t i, size_t j) { return blocks[i][j] == WATER_SHALLOW; });

		for (; total_river_area_left > 0;) {
			//std::cerr << "Total river area left: " << total_river_area_left << std::endl;

			// 7.2. Choose river border
			size_t cluster_number = possible_clusters[rand() % possible_clusters.size()];
			if (border_points[cluster_number].empty()) {
				possible_clusters.erase(std::find(possible_clusters.begin(), possible_clusters.end(), cluster_number));
				continue;
			}
			Point border = border_points[cluster_number][rand() % border_points[cluster_number].size()];

			// 7.3. Make path

			// 7.3.1. From source to border
			vector<Point> path = grid_function::FindClosest(neighbors, border, all_true, is_water);

			// 7.3.2. From border to another border
			const float MIN_DISTANCE = 20;
			vector<Point> border_to_border2 = grid_function::FindClosest(neighbors, border, on_border, is_water_shallow, MIN_DISTANCE);
			if (border_to_border2.empty())
				continue;
			path.insert(path.end(), border_to_border2.begin(), border_to_border2.end());

			// 7.3.3. From another border to sink
			Point border2 = border_to_border2.front();

			vector<Point> border2_to_sink = grid_function::FindClosest(neighbors, border2, all_true, is_water);
			path.insert(path.end(), border2_to_sink.begin(), border2_to_sink.end());

			// 7.4. Paint a river and decrease total_area_left
			for (Point point : path) {
				if (blocks[point] == WATER_SHALLOW)
					is_water_shallow[point] = false;
				blocks[point] = WATER;
				is_water[point] = true;
				--total_river_area_left;
				for (Point other : neighbors[point]) {
					if (on_border[other] && blocks[other] != WATER_DEEP) {
						total_river_area_left -= blocks[other] != WATER;
						is_water_shallow[other] = false;
						blocks[other] = WATER;
						is_water[other] = true;
					}
				}
			}

			for (Point point : path) {
				for (Point other : neighbors[point]) {
					if (blocks[other] == WATER) {
						for (Point very_other : neighbors[other]) {
							if (!WATER_TYPES.count(blocks[very_other])) {
								--total_river_area_left;
								blocks[very_other] = WATER_SHALLOW;
								is_water_shallow[very_other] = true;
							}
						}
					}
				}
			}
		}
	}
	time.PrintTime("Add rivers");
	*/

	// 11. Give subtypes to tiles
	for (uint32_t i = 0; i < height; ++i)
		for (uint32_t j = 0; j < width; ++j)
			blocks[i][j] = static_cast<BlockType>(GetSubtype(blocks[i][j]));

	// 12. Fill blocks_ array with the result from blocks
	for (uint32_t i = 0; i < height; ++i)
		for (uint32_t j = 0; j < width; ++j)
			blocks_[GetInd(j, i)] = blocks[i][j];

	time.PrintTime("Total time");
}
