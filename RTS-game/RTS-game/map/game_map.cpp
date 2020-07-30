#include "game_map.h"

#include <cassert>
#include <ctime>

#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "grid_function.h"

using grid_function::FromFunction;
using std::vector;

size_t GameMap::GetInd(uint32_t x, uint32_t y) const {
	return x + static_cast<size_t>(y) * width_;
}

GameMap::GameMap(SDL_Renderer* renderer, uint32_t width, uint32_t height) {
	width_ = width;
	height_ = height;

    blocks_.resize(static_cast<size_t>(width_) * height_);
	units_in_block_.resize(static_cast<size_t>(width_) * height_);

	// load texture
	SDL_Surface* surface = SDL_LoadBMP("pictures/test.bmp");
	tiles_ = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
}

GameMap::~GameMap() {

}

uint32_t GameMap::GetHeight() {
	return height_;
}

uint32_t GameMap::GetWidth() {
	return width_;
}

uint32_t GameMap::GetBlockSize() {
	return BLOCK_SIZE;
}

std::unordered_set<AbstractUnit*>* GameMap::GetUnitsInBlock(uint32_t x, uint32_t y) {
	return &units_in_block_[GetInd(x, y)];
}

bool GameMap::IsPositionInMap(int x, int y) {
	return (x >= 0 && y >= 0 &&
		    x < (BLOCK_SIZE * width_) && y < (BLOCK_SIZE * height_));
}

void GameMap::AddUnit(AbstractUnit* unit, uint32_t x, uint32_t y) {
	units_in_block_[GetInd(x, y)].insert(unit);
}

void GameMap::DeleteUnit(AbstractUnit* unit, uint32_t x, uint32_t y) {
	if (units_in_block_[GetInd(x, y)].find(unit) != units_in_block_[GetInd(x, y)].end()) {
		units_in_block_[GetInd(x, y)].erase(unit);
	}
}

uint8_t GameMap::GetSubtype(BlockType type, uint32_t x, uint32_t y) {
	if (type == BlockType::GRASS) {
		// 10 - 19
		return 10 + rand() % 10;
	}
	return type;
}

uint8_t GameMap::GetBlock(uint32_t x, uint32_t y) {
	return blocks_[GetInd(x, y)];
}

void GameMap::SetBlock(uint32_t x, uint32_t y, uint8_t value) {
	blocks_[GetInd(x, y)] = value;
}

void GameMap::BlockDraw(SDL_Renderer* renderer, Camera* camera, uint32_t x, uint32_t y) {
    uint8_t block = blocks_[GetInd(x, y)];
	if (!units_in_block_[GetInd(x, y)].empty()) {
		block = 0;
	}
	SDL_Rect from, to;
	from.x = (block % BLOCKS_IN_LINE) * BLOCK_SIZE;
	from.y = (block / BLOCKS_IN_LINE) * BLOCK_SIZE;
	from.w = BLOCK_SIZE;
	from.h = BLOCK_SIZE;
	to.x = - camera->GetCornerX(renderer) + x * BLOCK_SIZE;
	to.y = - camera->GetCornerY(renderer) + y * BLOCK_SIZE;
	to.w = BLOCK_SIZE;
	to.h = BLOCK_SIZE;
	SDL_RenderCopy(renderer, tiles_, &from, &to);
}

void GameMap::Draw(SDL_Renderer* renderer, Camera* camera) {
	//SDL_RenderCopy(renderer, tiles_, NULL, NULL);
	for (uint32_t y = 0; y < height_; ++y) {
		for (uint32_t x = 0; x < width_; ++x) {
			//uint32_t ind = x + y * width;
			BlockDraw(renderer, camera, x, y);
		}
	}
}

void GameMap::Generate() {
	std::time_t starting_time = time(0);

	uint32_t height = GetHeight(), width = GetWidth();
	// 1. Scattering random points (that will be centers of clusters)
	const uint32_t CHUNK_SIZE = 160;
	const uint32_t CHUNKS_COUNT = (height * width) / CHUNK_SIZE;

	srand(static_cast<unsigned int>(time(0)));

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

	/* 2. Building voronoi diagram
		How? Let's do dijkstra algorithm from all current points,
		then for each cell c we'll find the closest point,
		and c will belong to closest point's cluster
		That will take O(area * log(area)) (area = height_ * width_)
	*/
	grid_function::Dijkstra(&distance, &cluster);

	// 3. Give to each cluster random tile
	const vector<BlockType> allowed_blocks = { WATER, DESERT, GRASS_LIGHT, GRASS_DARK, GRASS };
	vector<BlockType> cluster_type{ CHUNKS_COUNT };
	for (uint32_t i = 0; i < CHUNKS_COUNT; ++i)
		cluster_type[i] = allowed_blocks[rand() % allowed_blocks.size()];

	// 4. Unite all small clusters with big ones
	{
		// 4.1. Find area of clusters
		vector<uint32_t> area(CHUNKS_COUNT, 0);
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

	// 5. Give the result to blocks grid (not array)
	Grid<BlockType> blocks = FromFunction<BlockType>(height, width, [&](size_t i, size_t j) {return cluster_type[cluster[i][j]]; });

	// Intermission. Make on_border grid
	Grid<bool> on_border(height, width, false);
	for (uint32_t i = 0; i < height; ++i) {
		for (uint32_t j = 0; j < width; ++j) {
			for (Point point : grid_function::GetNeighbors({ j, i }, height, width)) {
				on_border[i][j] = on_border[i][j] || (blocks[i][j] != blocks[point] &&
					blocks[i][j] != WATER && blocks[point] != WATER);
			}
		}
	}

	for (uint32_t i = 0; i < height; ++i) {
		for (uint32_t j = 0; j < width; ++j) {
			for (Point point : grid_function::GetNeighbors({ j, i }, height, width)) {
				on_border[i][j] = on_border[i][j] ||
					(blocks[i][j] == WATER && blocks[point] != WATER && on_border[point.y][point.x]);
			}
		}
	}

	// 6. Give subtypes to tiles
	for (uint32_t i = 0; i < height; ++i)
		for (uint32_t j = 0; j < width; ++j)
			blocks[i][j] = (BlockType) GetSubtype((BlockType)blocks[i][j], j, i);

	Grid<uint32_t> deep_water_area(height, width, 0);
	// Area of a deep water in corner-wise connectivity of that cell

	// 7. Make water look more deep
	{   // 7.1. Find distances from each water cell to the nearest land cell
		for (uint32_t i = 0; i < height; ++i)
			for (uint32_t j = 0; j < width; ++j)
				distance[i][j] = (blocks[i][j] == WATER) ? max_distance : 0;
		grid_function::Dijkstra(&distance);

		// 7.2. Make closest to land cells to have shallow water, similiar with farthest
		const uint32_t WATER_SHALLOW_MAX_DISTANCE = 2;
		const uint32_t WATER_DEEP_MIN_DISTANCE = 4;

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

		// 7.3. Remove deep water tiles with very low area
		const uint32_t DEEP_WATER_AREA_MIN = 6;
		Grid<bool> visited(height, width, false);
		for (uint32_t i = 0; i < height; ++i) {
			for (uint32_t j = 0; j < width; ++j) {
				if (blocks[i][j] == WATER_DEEP && !visited[i][j]) {
					vector<Point> bfs;
					bfs.push_back({ j, i });
					for (size_t i = 0; i < bfs.size(); ++i) {
						Point current = bfs[i];
						for (Point p : grid_function::GetNeighbors(current, height, width)) {
							if (blocks[p] == WATER_DEEP && !visited[p.y][p.x]) {
								bfs.push_back(p);
								visited[p.y][p.x] = true;
							}
						}
					}

					if (bfs.size() < DEEP_WATER_AREA_MIN) {
						for (Point point : bfs)
							blocks[point] = WATER;
					}
					else {
						for (Point point : bfs)
							deep_water_area[point] = static_cast<uint32_t>(bfs.size());
					}
				}
			}
		}
	}

	/*
	for (size_t i = 0; i < height; ++i)
		for (size_t j = 0; j < width; ++j)
			if (on_border[i][j])
				blocks[i][j] = (BlockType) 7;
	*/

	//8. Add rivers
	const std::unordered_set<BlockType> WATER_TYPES = { WATER_SHALLOW, WATER, WATER_DEEP };
	{
		// 8.1. Choose possible river sources
		const uint32_t MIN_DEEP_WATER_AREA = 10;
		std::unordered_set<int> possible_clusters_set;
		for (uint32_t i = 0; i < height; ++i)
			for (uint32_t j = 0; j < width; ++j)
				if (deep_water_area[i][j] > MIN_DEEP_WATER_AREA)
					possible_clusters_set.insert(cluster[i][j]);
		std::vector<int> possible_clusters(possible_clusters_set.begin(), possible_clusters_set.end());

		int total_river_area_left = 2500;
		if (possible_clusters.empty()) {  // No clusters => no rivers
			total_river_area_left = 0;
		}

		for (; total_river_area_left > 0;) {
			std::cerr << "Total river area left: " << total_river_area_left << std::endl;

			// 8.2. Choose river border
			size_t cluster_number = possible_clusters[rand() % possible_clusters.size()];
			vector<Point> border_points;
			for (uint32_t i = 0; i < height; ++i) {
				for (uint32_t j = 0; j < width; ++j) {
					if (cluster[i][j] == cluster_number && on_border[i][j])
						border_points.push_back({ j, i });
				}
			}
			if (border_points.empty())
				continue;
			Point border = border_points[rand() % border_points.size()];

			// 8.3. Make path
			Grid<bool> is_water = FromFunction<bool>(height, width,
				[&](size_t i, size_t j) { return blocks[i][j] == WATER; });
			Grid<bool> is_water_shallow = FromFunction<bool>(height, width,
				[&](size_t i, size_t j) { return blocks[i][j] == WATER_SHALLOW; });
			Grid<bool> is_chosen_cluster = FromFunction<bool>(height, width,
				[&](size_t i, size_t j) {return cluster[i][j] == cluster_number; });

			// 8.3.1. From source to border
			vector<Point> path = grid_function::FindClosest(border, is_chosen_cluster, is_water);
			reverse(path.begin(), path.end());

			// 8.3.2. From border to another border
			const float MIN_DISTANCE = 10;
			vector<Point> border_to_border2 = grid_function::FindClosest(border, on_border, is_water_shallow, MIN_DISTANCE);
			if (border_to_border2.empty())
				continue;
			path.insert(path.end(), border_to_border2.begin(), border_to_border2.end());

			// 8.3.3. From another border to sink
			Point border2 = path.back();
			is_chosen_cluster = FromFunction<bool>(height, width,
				[&](size_t i, size_t j) {return cluster[i][j] == cluster[border2]; });

			vector<Point> border2_to_sink = grid_function::FindClosest(border2, is_chosen_cluster, is_water);
			path.insert(path.end(), border2_to_sink.begin(), border2_to_sink.end());

			// 8.4. Paint a river and decrease total_area_left
			std::cerr << "Drawing a river from (" << path.front().x << ", " << path.front().y << ") to ("
				<< path.back().x << ", " << path.back().y << ")" << std::endl;

			for (const Point& point : path) {
				blocks[point] = WATER;
				--total_river_area_left;
				for (Point other : grid_function::GetNeighbors(point, height, width)) {
					if ((on_border[other.y][other.x] && !WATER_TYPES.count(blocks[other]))
						|| (blocks[other] == WATER_SHALLOW)) {
						total_river_area_left -= blocks[other] != WATER;
						blocks[other] = WATER;
					}
				}
			}
			for (const Point& point : path) {
				for (Point other : grid_function::GetNeighbors(point, height, width)) {
					if (blocks[other] == WATER) {
						for (Point very_other : grid_function::GetNeighbors(other, height, width)) {
							if (!WATER_TYPES.count(blocks[very_other])) {
								--total_river_area_left;
								blocks[very_other] = WATER_SHALLOW;
							}
						}
					}
				}
			}
		}
	}

	// 9. Add mountains
	// 10. Add passages between mountains
	// 11. Add trees

	// 12. Fill blocks_ array with the result from blocks
	for (uint32_t i = 0; i < height; ++i)
		for (uint32_t j = 0; j < width; ++j)
			blocks_[GetInd(j, i)] = blocks[i][j];
	
	std::cerr << "Building map time: " << (time(0) - starting_time) << " s" << std::endl;
}
