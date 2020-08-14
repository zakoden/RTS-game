#include "game_map.h"

#include <cassert>

#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "diamond_square.h"
#include "grid_function.h"
#include "grid_neighbors.h"
#include "perlin.h"
#include "time_measurer.h"
#include "triple.h"

using grid_function::FromFunction;
using std::vector;

// Returns (random number in [l; r])% of area
inline size_t GetRandomArea(size_t l, size_t r, size_t area) {
	return area * (rand() % (r - l + 1) + l) / 100;
}

uint8_t GameMap::GetSubtype(BlockType type) {
	switch (type) {
		case GRASS:
			return GRASS + rand() % 10;
		case GRASS_PURPLE:
			return GRASS_PURPLE + rand() % 10;
		case DESERT:
			return DESERT + rand() % 3;
		case DESERT_PURPLE:
			return DESERT_PURPLE + rand() % 4;
		case MOUNTAIN_HIGH:
			return MOUNTAIN_HIGH + rand() % 4;
		case MOUNTAIN_LOW:
			return MOUNTAIN_LOW + rand() % 4;
		case ROCK:
			return ROCK + rand() % 6;
	}
	return type;
}

// Returns a block by height and humidity
inline BlockType GetBlockType(float height, float humidity) {
	height += 1; humidity += 0.75;
	/* Height types:
	0. From WATER_SHALLOW_LEVEL to 1	2. From 1 to 1.1
	1. From 1 to 1.1					3. From 1.2 to 2
	*/
	size_t height_type = static_cast<size_t>(height > 1) + (height > 1.1) + (height > 1.2);
	/* Humidity types:
	0. From 0 to 0.5		3. From 1 to 1.333
	1. From 0.5 to 0.667	4. From 1.333 to 1.667
	2. From 0.667 to 1		5. From 1.667 to 2
	*/
	size_t humidity_type = static_cast<size_t>(humidity > 0.5) + (humidity > 0.667)
		+ (humidity > 1) + (humidity > 1.333) + (humidity > 1.667);

	// Biome diagram
	const vector<vector<BlockType>> diagram = {
		{SUBTROPICAL_DESERT, GRASSLAND, TROPICAL_SEASONAL_FOREST, TROPICAL_SEASONAL_FOREST, TROPICAL_RAIN_FOREST, TROPICAL_RAIN_FOREST},
		{TEMPERATE_DESERT, GRASS_PURPLE, GRASSLAND, TEMPERATE_DECIDUOUS_FOREST, TEMPERATE_DECIDUOUS_FOREST, TEMPERATE_RAIN_FOREST},
		{TEMPERATE_DESERT, DESERT_PURPLE, SHRUBLAND, SHRUBLAND, TAIGA, TAIGA},
		{SCORCHED, BARE, TUNDRA, SNOW, SNOW, SNOW}
	};
	return diagram[height_type][humidity_type];
}

// Generates a map of heights
Grid<float> GameMap::GenerateHeights() {
	uint32_t height = GetHeight(), width = GetWidth();
	float power = 1;
	float max_height = 0;
	Grid<float> result(height, width, 0);
	for (uint32_t cur_size = height / 2; cur_size <= height; cur_size <<= 1) {
		Grid<float> result_perlin = perlin::GetHeights(cur_size, cur_size);
		Grid<float> result_diamond_square = diamond_square::GetHeights(cur_size, cur_size);
		uint32_t height_divider = height / cur_size, width_divider = width / cur_size;
		for (uint32_t i = 0; i < height; ++i)
			for (uint32_t j = 0; j < width; ++j)
				result[i][j] += power * (
					result_perlin[i / height_divider][j / width_divider]
					+ result_diamond_square[i / height_divider][j / width_divider]
					);
		max_height += power * 2.0f;
		power /= 2.0f;
	}

	float average_height = 0;
	for (uint32_t i = 0; i < height; ++i) {
		for (uint32_t j = 0; j < width; ++j) {
			result[i][j] /= max_height;
			average_height += result[i][j];
		}
	}
	average_height /= height * width;
	for (uint32_t i = 0; i < height; ++i)
		for (uint32_t j = 0; j < width; ++j)
			result[i][j] -= average_height;

	return result;
}

void GameMap::Generate() {
	unsigned int seed = static_cast<unsigned int>(time(0));  // Map seed
	TimeMeasurer time;  // Class to measure time between each segment
	srand(seed);  // Randomizing rand
	std::cerr << "Seed is " << seed << std::endl;
	const std::unordered_set<BlockType> WATER_TYPES = { WATER_SHALLOW, WATER, WATER_DEEP };
	const std::unordered_set<BlockType> MOUNTAIN_TYPES = { SCORCHED, BARE, TUNDRA, SNOW, SNOW, SNOW };
	uint32_t height = GetHeight(), width = GetWidth();  // Height and width of a map
	const size_t TOTAL_AREA = static_cast<size_t>(height) * width;  // Total area of a map
	GridNeighbors neighbors(height, width);  // Diagonal-wise neighbors of each cell

	// Give the result to blocks grid
	Grid<BlockType> blocks(height, width);

	// Build water and mountains via mixed noise between diamond square and perlin
	Grid<float> heights = GenerateHeights();
	Grid<float> humidity = GenerateHeights();
	{
		const float MOUNTAIN_HEIGHT = 0.2f;
		const float WATER_NORMAL_LEVEL = -0.25f;
		const float WATER_DEEP_LEVEL = -0.35f, WATER_SHALLOW_LEVEL = -0.18f;

		for (uint32_t i = 0; i < height; ++i) {
			for (uint32_t j = 0; j < width; ++j) {
				if (heights[i][j] < WATER_DEEP_LEVEL)
					blocks[i][j] = WATER_DEEP;
				else if (heights[i][j] < WATER_NORMAL_LEVEL)
					blocks[i][j] = WATER;
				else if (heights[i][j] < WATER_SHALLOW_LEVEL)
					blocks[i][j] = WATER_SHALLOW;
				else
					blocks[i][j] = GetBlockType(heights[i][j], humidity[i][j]);
			}
		}
	}

	time.PrintTime("Build water and mountains");
	

	// For each cell, if 5 neighbors have the same block type, transform that cell into that block type
	for (uint32_t round = 1; round <= 3; ++round) {
		uint32_t transformed_cells = 0;
		for (uint32_t i = 0; i < height; ++i) {
			for (uint32_t j = 0; j < width; ++j) {
				std::unordered_map<BlockType, int> blocks_count;
				for (Point point : neighbors[i][j])
					++blocks_count[blocks[point]];
				for (auto& pair : blocks_count) {
					if (pair.second >= 5) {
						transformed_cells += blocks[i][j] != pair.first;
						blocks[i][j] = pair.first;
						break;
					}
				}
			}
		}
		std::cerr << "Round #" << round << ": Transformed " << transformed_cells << " cells" << std::endl;
	}
	time.PrintTime("Flatten map");

	// Unite small areas with the closest non-small ones
	const uint32_t AREA_MIN = 80;  // Minimal area each biome should have
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
	time.PrintTime("Unite small areas with the closest non-small ones");

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

	// Get statistics
	std::cerr << "Total area: " << TOTAL_AREA << std::endl;
	size_t water_area = 0, mountain_area = 0;
	for (uint32_t i = 0; i < height; ++i) {
		for (uint32_t j = 0; j < width; ++j) {
			water_area += WATER_TYPES.count(blocks[i][j]);
			mountain_area += MOUNTAIN_TYPES.count(blocks[i][j]);
		}
	}
	std::cerr << "Mountain area: " << mountain_area << " = " << (mountain_area * 100 / height / width) << "%" << std::endl;
	std::cerr << "Water area: " << water_area << " = " << (water_area * 100 / height / width) << "%" << std::endl;

	// Give subtypes to tiles
	for (uint32_t i = 0; i < height; ++i)
		for (uint32_t j = 0; j < width; ++j)
			blocks[i][j] = static_cast<BlockType>(GetSubtype(blocks[i][j]));

	// Fill blocks_ array with the result from blocks
	for (uint32_t i = 0; i < height; ++i)
		for (uint32_t j = 0; j < width; ++j)
			blocks_[GetInd(j, i)] = blocks[i][j];

	time.PrintTime("Total time");
}
