#include "game_map.h"

#include <cassert>
#include <chrono>
#include <ctime>

#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "grid_function.h"
#include "grid_neighbors.h"
#include "time_measurer.h"

using grid_function::FromFunction;
using std::vector;

void GameMap::Generate() {
	srand(static_cast<unsigned int>(time(0)));
	TimeMeasurer time;

	uint32_t height = GetHeight(), width = GetWidth();
	GridNeighbors neighbors{ height, width };

	// 1. Scattering random points (that will be centers of clusters)
	const uint32_t CHUNK_SIZE = 400;
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

	// 3. Give to each cluster random tile
	const vector<BlockType> ALLOWED_BLOCKS = { WATER, DESERT, GRASS_LIGHT, GRASS, GRASS_OTHER };
	const vector<BlockType> RARE_BLOCKS = { AMBER, GRASS_PURPLE, DESERT_PURPLE };
	vector<BlockType> cluster_type{ CHUNKS_COUNT };
	for (uint32_t i = 0; i < CHUNKS_COUNT; ++i) {
		if (rand() % 5 == 0)
			cluster_type[i] = RARE_BLOCKS[rand() % RARE_BLOCKS.size()];
		else
			cluster_type[i] = ALLOWED_BLOCKS[rand() % ALLOWED_BLOCKS.size()];
	}

	time.PrintTime("Give to each cluster random tile");
	// 4. Unite all small clusters with big ones
	vector<uint32_t> area(CHUNKS_COUNT, 0);
	{
		// 4.1. Find area of clusters
		for (uint32_t i = 0; i < height; ++i)
			for (uint32_t j = 0; j < width; ++j)
				++area[cluster[i][j]];

		// 4.2 Remove chunks with area < AREA_MIN
		const size_t AREA_MIN = 80;
		std::unordered_map<uint32_t, uint32_t> chunks_to_remove;
		for (size_t i = 0; i < area.size(); ++i)
			if (area[i] < AREA_MIN)
				chunks_to_remove.insert({ i, 0 });
		std::cerr << "Removed " << chunks_to_remove.size() << " clusters" << std::endl;

		// 4.3 Replace removed chunk with the closest ones
		for (auto& pair : chunks_to_remove) {
			uint32_t from = pair.first;
			uint32_t min_distance = UINT_MAX;
			for (uint32_t i = 0; i < CHUNKS_COUNT; ++i) {
				if (!chunks_to_remove.count(i)) {
					uint32_t distance = grid_function::SquaredDistance(centers[from], centers[i]);
					if (min_distance < distance) {
						min_distance = distance;
						pair.second = i;
					}
				}
			}
		}

		// 4.3. Reflect changes on a map
		for (uint32_t i = 0; i < height; ++i)
			for (uint32_t j = 0; j < width; ++j)
				if (chunks_to_remove.count(cluster[i][j]))
					cluster[i][j] = chunks_to_remove[cluster[i][j]];
	}

	time.PrintTime("Unite all small clusters with big ones");
	// 5. Give the result to blocks grid (not array)
	Grid<BlockType> blocks = FromFunction<BlockType>(height, width, [&](size_t i, size_t j) {return cluster_type[cluster[i][j]]; });
	time.PrintTime("Give the result to blocks grid");

	// Intermission. Make on_border grid
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
	time.PrintTime("Make on_border grid");
	// 6. Make water look more deep
	// Area of a deep water in corner-wise connectivity of that cell
	Grid<uint32_t> deep_water_area(height, width, 0);
	{   // 6.1. Find distances from each water cell to the nearest land cell
		for (uint32_t i = 0; i < height; ++i)
			for (uint32_t j = 0; j < width; ++j)
				distance[i][j] = (blocks[i][j] == WATER) ? max_distance : 0;
		grid_function::Dijkstra(neighbors, &distance);

		// 6.2. Make closest to land cells to have shallow water, similiar with farthest
		const float WATER_SHALLOW_MAX_DISTANCE = 1.5;
		const float WATER_DEEP_MIN_DISTANCE = 3.5;

		for (uint32_t i = 0; i < height; ++i) {
			for (uint32_t j = 0; j < width; ++j) {
				if (blocks[i][j] == WATER) {
					if (distance[i][j] <= WATER_SHALLOW_MAX_DISTANCE)
						blocks[i][j] = WATER_SHALLOW;
					else if (distance[i][j] >= WATER_DEEP_MIN_DISTANCE)
						blocks[i][j] = WATER_DEEP;
				}
			}
		}
		time.PrintTime("Before 6.3");

		// 6.3. Remove deep water tiles with very low area
		const uint32_t DEEP_WATER_AREA_MIN = 6;
		deep_water_area = grid_function::GetAreas(neighbors, blocks);
		for (uint32_t i = 0; i < height; ++i)
			for (uint32_t j = 0; j < width; ++j)
				if (blocks[i][j] == WATER_DEEP && deep_water_area[i][j] < DEEP_WATER_AREA_MIN)
					blocks[i][j] = WATER;
	}


	time.PrintTime("Make water look more deep");

	//7. Add rivers
	const std::unordered_set<BlockType> WATER_TYPES = { WATER_SHALLOW, WATER, WATER_DEEP };
	{
		// 8.1. Choose possible river sources
		const uint32_t MIN_DEEP_WATER_AREA = 10;
		std::unordered_set<int> possible_clusters_set;
		for (uint32_t i = 0; i < height; ++i)
			for (uint32_t j = 0; j < width; ++j)
				if (deep_water_area[i][j] > MIN_DEEP_WATER_AREA)
					possible_clusters_set.insert(cluster[i][j]);
		vector<int> possible_clusters(possible_clusters_set.begin(), possible_clusters_set.end());

		vector<vector<Point>> border_points(CHUNKS_COUNT);
		for (uint32_t i = 0; i < height; ++i)
			for (uint32_t j = 0; j < width; ++j)
				if (on_border[i][j])
					border_points[cluster[i][j]].push_back({ j, i });

		int total_river_area_left = 2500;
		if (possible_clusters.empty())  // No clusters => no rivers
			total_river_area_left = 0;

		Grid<char> all_true(height, width, true);
		Grid<char> is_water = FromFunction<char>(height, width,
			[&](size_t i, size_t j) { return blocks[i][j] == WATER; });
		Grid<char> is_water_shallow = FromFunction<char>(height, width,
			[&](size_t i, size_t j) { return blocks[i][j] == WATER_SHALLOW; });

		for (; total_river_area_left > 0;) {
			//std::cerr << "Total river area left: " << total_river_area_left << std::endl;

			// 8.2. Choose river border
			size_t cluster_number = possible_clusters[rand() % possible_clusters.size()];
			if (border_points[cluster_number].empty()) {
				possible_clusters.erase(std::find(possible_clusters.begin(), possible_clusters.end(), cluster_number));
				continue;
			}
			Point border = border_points[cluster_number][rand() % border_points[cluster_number].size()];

			// 8.3. Make path

			// 8.3.1. From source to border
			vector<Point> path = grid_function::FindClosest(neighbors, border, all_true, is_water);
			reverse(path.begin(), path.end());

			// 8.3.2. From border to another border
			const float MIN_DISTANCE = 20;
			vector<Point> border_to_border2 = grid_function::FindClosest(neighbors, border, on_border, is_water_shallow, MIN_DISTANCE);
			if (border_to_border2.empty())
				continue;
			path.insert(path.end(), border_to_border2.begin(), border_to_border2.end());

			// 8.3.3. From another border to sink
			Point border2 = path.back();

			vector<Point> border2_to_sink = grid_function::FindClosest(neighbors, border2, all_true, is_water);
			path.insert(path.end(), border2_to_sink.begin(), border2_to_sink.end());

			// 8.4. Paint a river and decrease total_area_left
			std::cerr << "Drawing a river from (" << path.front().x << ", " << path.front().y << ") to ("
				<< path.back().x << ", " << path.back().y << ")" << std::endl;

			for (const Point& point : path) {
				blocks[point] = WATER;
				is_water[point] = true;
				--total_river_area_left;
				for (Point other : neighbors[point]) {
					if (on_border[other.y][other.x] && !WATER_TYPES.count(blocks[other])) {
						total_river_area_left -= blocks[other] != WATER;
						blocks[other] = WATER;
						is_water[other] = true;
					}
					else if (blocks[other] == WATER_SHALLOW) {
						blocks[other] = WATER;
						is_water_shallow[other] = false;
						is_water[other] = true;
					}
				}
			}
			for (const Point& point : path) {
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


	// 8. Add mountains
	{
		Grid<BlockType> previous_blocks = blocks;
		/* 8.1. Add actual mountains
		How? Let's go through all borders, pick part of the border that is on the larger cluster,
		then make all cells in that border to be mountains
		*/
		for (uint32_t i = 0; i < height; ++i) {
			for (uint32_t j = 0; j < width; ++j) {
				if (on_border[i][j] && blocks[i][j] != WATER) {
					char has_area_bigger = false;
					for (Point point : neighbors[i][j]) {
						if (blocks[i][j] != blocks[point] &&
							blocks[i][j] != WATER && blocks[point] != WATER
							&& blocks[i][j] != blocks[point]) {
							if (area[cluster[i][j]] < area[cluster[point]]) {
								has_area_bigger = true;
								break;
							}
						}
					}

					if (has_area_bigger)
						continue;

					blocks[i][j] = MOUNTAIN_HIGH;
					for (Point point : neighbors[i][j]) {
						if (blocks[point] == WATER_SHALLOW)
							blocks[point] = MOUNTAIN_HIGH;
					}
				}
			}
		}

		// 8.2. Remove mountains that don't separate 2 non-water biomes
		for (uint32_t i = 0; i < height; ++i) {
			for (uint32_t j = 0; j < width; ++j) {
				if (blocks[i][j] == MOUNTAIN_HIGH) {
					std::unordered_set<BlockType> neighbor_types;
					for (Point point : neighbors[i][j]) {
						if (!WATER_TYPES.count(blocks[point]) && blocks[point] != MOUNTAIN_HIGH)
							neighbor_types.insert(blocks[point]);
					}
					if (neighbor_types.size() < 2)  // Should be removed
						blocks[i][j] = previous_blocks[i][j];
				}
			}
		}


		/* 8.3. Make lower mountains
		How? Let's go through all high mountains, and
		if the neighbor is not water and not mountain, then it's a lower mountain
		*/
		for (uint32_t i = 0; i < height; ++i) {
			for (uint32_t j = 0; j < width; ++j) {
				if (blocks[i][j] == MOUNTAIN_HIGH) {
					for (Point point : neighbors[i][j]) {
						if (blocks[point] != MOUNTAIN_HIGH && blocks[point] != WATER)
							blocks[point] = MOUNTAIN_LOW;
					}
				}
			}
		}
	}

	// 9. Clear small areas
	{
		// 9.1. Remove small areas
		const uint32_t MIN_AREA = 100;
		Grid<uint32_t> areas = grid_function::GetAreas(neighbors, blocks);
		for (uint32_t i = 0; i < height; ++i) {
			for (uint32_t j = 0; j < width; ++j) {
				if (areas[i][j] < MIN_AREA) {
					const uint32_t MIN_WATER_SHALLOW_AREA = 8;
					if (blocks[i][j] == WATER_SHALLOW) {
						if (areas[i][j] < MIN_WATER_SHALLOW_AREA)
							blocks[i][j] = MOUNTAIN_HIGH;
					}
					else {
						if (blocks[i][j] != MOUNTAIN_HIGH && blocks[i][j] != MOUNTAIN_LOW
							&& !WATER_TYPES.count(blocks[i][j]))
							blocks[i][j] = MOUNTAIN_HIGH;
					}
				}
			}
		}

		// 9.2. Make shallow water cell a normal one if it's not near land blocks
		const std::unordered_set<BlockType> MOUNTAIN_TYPES = { MOUNTAIN_LOW, MOUNTAIN_HIGH };
		for (uint32_t i = 0; i < height; ++i) {
			for (uint32_t j = 0; j < width; ++j) {
				if (blocks[i][j] == WATER_SHALLOW) {
					bool remove = true;
					for (Point point : neighbors[i][j]) {
						if (!WATER_TYPES.count(blocks[point]) && !MOUNTAIN_TYPES.count(blocks[point])) {
							remove = false;
							break;
						}
					}
					if (remove)
						blocks[i][j] = WATER;
				}
			}
		}
	}

	// 9. Add passages between mountains
	for (uint32_t i = 0; i < height; ++i) {

	}

	// 10. Add trees

	// 11. Give subtypes to tiles
	for (uint32_t i = 0; i < height; ++i) {
		for (uint32_t j = 0; j < width; ++j) {
			blocks[i][j] = (BlockType)GetSubtype((BlockType)blocks[i][j], j, i);
		}
	}

	// 12. Fill blocks_ array with the result from blocks
	for (uint32_t i = 0; i < height; ++i)
		for (uint32_t j = 0; j < width; ++j)
			blocks_[GetInd(j, i)] = blocks[i][j];

	time.PrintTime("Total time");
}
