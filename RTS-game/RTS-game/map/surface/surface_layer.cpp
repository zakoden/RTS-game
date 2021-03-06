#include "surface_layer.h"

#include <cassert>

#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "diamond_square.h"
#include "perlin.h"

#include "../grid/grid_function.h"
#include "../grid/grid_neighbors.h"
#include "../time_measurer.h"
#include "../triple.h"

using grid_function::FromFunction;
using std::vector;

// Get subtype of a block
inline uint8_t GetSubtype(BlockType type) {
	switch (type) {
	case WATER_DEEP:
		return WATER_DEEP;
	case WATER:
		return WATER + rand() % 4;
	case WATER_SHALLOW:
		return WATER_SHALLOW + rand() % 4;
	case DESERT:
		return DESERT + rand() % 3;
	case GRASS_PURPLE:
		if (rand() % 50)
			return GRASS_PURPLE + rand() % 10;
		else
			return 110 + rand() % 6;
	case DESERT_PURPLE:
		return DESERT_PURPLE + rand() % 4;
	case GRASS:
		return GRASS + rand() % 10;
	case GRASS_TROPICAL:
		return GRASS_TROPICAL + rand() % 4;
	case MOUNTAIN_LOW:
		return MOUNTAIN_LOW + rand() % 4;
	case MOUNTAIN_HIGH:
		return MOUNTAIN_HIGH + rand() % 4;

	default:
		return type;
	}
}

// Returns a block by height and humidity
inline BlockType GetBlockType(float height, float humidity, const vector<vector<BlockType>>& DIAGRAM) {
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

	return DIAGRAM[height_type][humidity_type];
}

// Generates a map of heights
Grid<float> GenerateHeights(uint32_t height, uint32_t width) {
	assert(height >= 2);
	float power = 1;
	float max_height = 0;
	Grid<float> result(height, width, 0);
	for (uint32_t cur_size = height / 2; cur_size <= height; cur_size <<= 1) {
		Grid<float> result_perlin = perlin::GetHeights(cur_size, cur_size);
		Grid<float> result_diamond_square = diamond_square::GetHeights(cur_size, cur_size);
		uint32_t height_divider = height / cur_size, width_divider = width / cur_size;
		for (uint32_t i = 0; i < height; ++i) {
			uint32_t ind_i = i / height_divider;
			for (uint32_t j = 0; j < width; ++j) {
				result[i][j] += power * (
					result_perlin[ind_i][j / width_divider]
					+ result_diamond_square[ind_i][j / width_divider]
					);
			}
		}
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

void SurfaceLayer::Generate(uint64_t seed) {
	//1597431138, 1597486519, 1598023852
	TimeMeasurer time, time_total = time;  // Class to measure time between each segment

	srand(seed);  // Randomizing rand
	std::cerr << "SurfaceLayer::Generate: Map seed is " << seed << std::endl;

	const std::unordered_set<BlockType> WATER_TYPES = { WATER_SHALLOW, WATER, WATER_DEEP };
	const std::unordered_set<BlockType> MOUNTAIN_TYPES = { SCORCHED, BARE, TUNDRA, SNOW };
	const vector<vector<BlockType>> DIAGRAM = {  // Diagram to determine biome type, for GetBlockType
		{SUBTROPICAL_DESERT, GRASSLAND, TROPICAL_SEASONAL_FOREST, TROPICAL_SEASONAL_FOREST, TROPICAL_RAIN_FOREST, TROPICAL_RAIN_FOREST},
		{TEMPERATE_DESERT, GRASS_PURPLE, GRASSLAND, TEMPERATE_DECIDUOUS_FOREST, TEMPERATE_DECIDUOUS_FOREST, TEMPERATE_RAIN_FOREST},
		{TEMPERATE_DESERT, DESERT_PURPLE, SHRUBLAND, SHRUBLAND, TAIGA, TAIGA},
		{SCORCHED, BARE, TUNDRA, SNOW, SNOW, SNOW}
	};

	uint32_t height = GetHeight(), width = GetWidth();  // Height and width of a map
	const size_t TOTAL_AREA = static_cast<size_t>(height) * width;  // Total area of a map
	time.PrintTime("SurfaceLayer::Generate: Generate constants");

	GridNeighbors neighbors(height, width);  // Diagonal-wise neighbors of each cell
	time.PrintTime("SurfaceLayer::Generate: Generate neighbors");

	// Give the result to blocks grid
	Grid<BlockType> blocks(height, width);

	// Build water and mountains via mixed noise between diamond square and perlin
	Grid<float> heights = GenerateHeights(height, width);
	Grid<float> humidity = GenerateHeights(height, width);
	time.PrintTime("SurfaceLayer::Generate: Get heights and humidity");
	{
		const float WATER_NORMAL_LEVEL = -0.264f;
		const float WATER_DEEP_LEVEL = -0.35f, WATER_SHALLOW_LEVEL = -0.2f;

		for (uint32_t i = 0; i < height; ++i) {
			for (uint32_t j = 0; j < width; ++j) {
				if (heights[i][j] < WATER_DEEP_LEVEL)
					blocks[i][j] = WATER_DEEP;
				else if (heights[i][j] < WATER_NORMAL_LEVEL)
					blocks[i][j] = WATER;
				else if (heights[i][j] < WATER_SHALLOW_LEVEL)
					blocks[i][j] = WATER_SHALLOW;
				else
					blocks[i][j] = GetBlockType(heights[i][j], humidity[i][j], DIAGRAM);
			}
		}
	}

	time.PrintTime("SurfaceLayer::Generate: Build water and mountains");

	// Smooth map 1 time
	grid_function::SmoothMap(neighbors, blocks);
	time.PrintTime("SurfaceLayer::Generate: Smooth map 1 time");

	// Remove small areas
	const uint32_t AREA_MIN = 80;  // Minimal area each biome should have
	grid_function::RemoveSmallAreas(neighbors, blocks, AREA_MIN);
	time.PrintTime("SurfaceLayer::Generate: Unite small areas with the closest non-small ones");

	// Make on_border grid
	Grid<char> on_border(height, width, false);
	{
		for (uint32_t i = 0; i < height; ++i) {
			for (uint32_t j = 0; j < width; ++j) {
				for (MapPoint point : neighbors[i][j]) {
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
				for (MapPoint point : neighbors[i][j]) {
					if (blocks[i][j] == WATER && blocks[point] != WATER && on_border[point.y][point.x]) {
						on_border[i][j] = true;
						break;
					}
				}
			}
		}
	}
	time.PrintTime("SurfaceLayer::Generate: Make on_border grid");
	// Add rivers
	{
		// 1. Add points on a border
		vector<MapPoint> border_points;
		for (uint32_t i = 0; i < height; ++i)
			for (uint32_t j = 0; j < width; ++j)
				if (on_border[i][j] && WATER_TYPES.count(blocks[i][j]))
					border_points.push_back({ j, i });

		int total_river_area_left = 2000;

		Grid<char> is_water_type = FromFunction<char>(height, width,
			[&](size_t i, size_t j) { return static_cast<char>(WATER_TYPES.count(blocks[i][j])); });
		Grid<char> all_true(height, width, true);
		Grid<char> is_water = FromFunction<char>(height, width,
			[&](size_t i, size_t j) { return blocks[i][j] == WATER; });
		Grid<char> is_water_shallow = FromFunction<char>(height, width,
			[&](size_t i, size_t j) { return blocks[i][j] == WATER_SHALLOW; });

		for (; total_river_area_left > 0;) {
			std::cerr << "SurfaceLayer::Generate: Total river area left: " << total_river_area_left << std::endl;

			// 2. Choose river border
			MapPoint border = border_points[rand() % border_points.size()];

			// 3. Make path

			// 3.1. From source to border
			vector<MapPoint> path = grid_function::FindClosest(neighbors, border, all_true, is_water, 10);

			// 3.2. From border to another border
			vector<MapPoint> border_to_border2 = grid_function::FindFarthest(neighbors, border, on_border, is_water_shallow);
			if (border_to_border2.empty()) {
				std::cerr << "SurfaceLayer::Generate: Couldn't find another border" << std::endl;
				continue;
			}
			path.insert(path.end(), border_to_border2.begin(), border_to_border2.end());

			// 3.3. From another border to sink
			MapPoint border2 = border_to_border2.front();

			vector<MapPoint> border2_to_sink = grid_function::FindClosest(neighbors, border2, all_true, is_water, 10);
			path.insert(path.end(), border2_to_sink.begin(), border2_to_sink.end());

			// 4. Paint a river and decrease total_area_left
			for (MapPoint point : path) {
				total_river_area_left -= !WATER_TYPES.count(blocks[point]);
				is_water_shallow[point] = false;
				if (blocks[point] != WATER_DEEP) {
					blocks[point] = WATER;
					is_water[point] = true;
				}
				for (MapPoint other : neighbors[point]) {
					if (blocks[other] != WATER_DEEP /*&& (on_border[other] || blocks[other] == WATER_SHALLOW)*/) {
						total_river_area_left -= !WATER_TYPES.count(blocks[point]);
						is_water_shallow[other] = false;
						blocks[other] = WATER;
						is_water[other] = true;
					}
				}
			}

			for (MapPoint point : path) {
				for (MapPoint other : neighbors[point]) {
					if (blocks[other] == WATER) {
						for (MapPoint very_other : neighbors[other]) {
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
	time.PrintTime("SurfaceLayer::Generate: Add rivers");

	// Smooth map 2 more times and remove small areas
	grid_function::SmoothMap(neighbors, blocks);
	grid_function::SmoothMap(neighbors, blocks);
	time.PrintTime("SurfaceLayer::Generate: Smooth map 2 more times");
	grid_function::RemoveSmallAreas(neighbors, blocks, AREA_MIN);
	time.PrintTime("SurfaceLayer::Generate: Remove small areas again");

	// Get statistics
	std::cerr << "SurfaceLayer::Generate: Total area: " << TOTAL_AREA << std::endl;
	size_t water_area = 0, mountain_area = 0;
	for (uint32_t i = 0; i < height; ++i) {
		for (uint32_t j = 0; j < width; ++j) {
			//if (on_border[i][j]) blocks[i][j] = (BlockType)9;
			water_area += WATER_TYPES.count(blocks[i][j]);
			mountain_area += MOUNTAIN_TYPES.count(blocks[i][j]);
		}
	}
	std::cerr << "SurfaceLayer::Generate: Mountain area: " << mountain_area 
		<< " (" << (mountain_area * 100 / height / width) << "%)" << std::endl;
	std::cerr << "SurfaceLayer::Generate: Water area: " << water_area 
		<< " (" << (water_area * 100 / height / width) << "%)" << std::endl;

	// Give subtypes to tiles
	for (uint32_t i = 0; i < height; ++i)
		for (uint32_t j = 0; j < width; ++j)
			blocks[i][j] = static_cast<BlockType>(GetSubtype(blocks[i][j]));

	// Fill blocks_ array with the result from blocks
	for (uint32_t i = 0; i < height; ++i)
		for (uint32_t j = 0; j < width; ++j)
			blocks_[GetInd(j, i)] = blocks[i][j];

	time_total.PrintTime("SurfaceLayer::Generate: Total time");
}