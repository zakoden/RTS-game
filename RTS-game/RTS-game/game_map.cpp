#include "game_map.h"

#include <cassert>
#include <ctime>

#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>

#include "grid_function.h"

using grid_function::operator==;
using grid_function::Point;
using std::vector;

size_t GameMap::GetInd(uint32_t x, uint32_t y) {
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
		    (uint32_t)x < (BLOCK_SIZE * width_) && (uint32_t)y < (BLOCK_SIZE * height_));
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
	if (type == BlockType::WATER_DEEP) {
		return 0;
	}
	if (type == BlockType::WATER) {
		return 1;
	}
	if (type == BlockType::WATER_SHALLOW) {
		return 2;
	}
	if (type == BlockType::DESERT) {
		return 70 + rand() % 3;
	}
	if (type == BlockType::GRASS) {
		return 10 + rand() % 9;
	}
	if (type == BlockType::GRASS_PURPLE) {
		return 20 + rand() % 10;
	}
	if (type == BlockType::MOUNTAIN_LOW) {
		return 30 + rand() % 6;
	}
	if (type == BlockType::MOUNTAIN_HIGH) {
		return 40 + rand() % 4;
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

// Generates random map
void GameMap::Generate() {
	uint32_t height = GetHeight(), width = GetWidth();
	// 1. Scattering random points (that will be centers of clusters)
	const uint32_t CHUNK_SIZE = 160;
	const uint32_t CHUNKS_COUNT = (height * width) / CHUNK_SIZE;

	srand(static_cast<unsigned int>(time(0)));

	float max_distance = static_cast<float>(height + width);
	// A suboptimal distance between opposing corners

	// Distance between some point and closest cluster point
	vector<vector<float>> distance(height, vector<float>(width, max_distance));

	// To which cluster this point belongs (-1 for "no cluster")
	vector<vector<int>> cluster(height, vector<int>(width, -1));

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
	const vector<BlockType> allowed_blocks = { WATER, DESERT, GRASS, GRASS_PURPLE, MOUNTAIN_LOW, MOUNTAIN_HIGH};
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
					uint32_t distance = SquaredDistance(centers[from], centers[i]);
					if (min_distance < distance) {
						min_distance = distance;
						pair.second = i;
					}
				}
			}
		}

		// 4.3. Reflect changes on a map
		for (uint32_t i = 0; i < height; ++i) {
			for (uint32_t j = 0; j < width; ++j) {
				if (chunks_to_remove.count(cluster[i][j])) {
					cluster[i][j] = chunks_to_remove[cluster[i][j]];
				}
			}
		}
	}

	// -3. Give the result to blocks_ array
	for (uint32_t i = 0; i < height; ++i) {
		for (uint32_t j = 0; j < width; ++j) {
			blocks_[GetInd(j, i)] = cluster_type[cluster[i][j]];
			//std::cout << cluster[j][i] << std::endl;
		}
	}

	// -2. Give subtypes to tiles
	for (uint32_t i = 0; i < height; ++i) {
		for (uint32_t j = 0; j < width; ++j) {
			blocks_[GetInd(j, i)] = GetSubtype((BlockType)blocks_[GetInd(j, i)], j, i);
		}
	}

	// -1. Make water look more deep
	// -1.1. Find distances from each water cell to the nearest land cell
	{
		for (uint32_t i = 0; i < height; ++i) {
			for (uint32_t j = 0; j < width; ++j) {
				distance[i][j] = (blocks_[GetInd(j, i)] == WATER) ? max_distance : 0;
			}
		}
		grid_function::Dijkstra(&distance, nullptr);

		// -1.2. Make closest to land cells to have shallow water, similiar with farthest
		const uint32_t WATER_SHALLOW_MAX_DISTANCE = 2;
		const uint32_t WATER_DEEP_MIN_DISTANCE = 4;

		for (uint32_t i = 0; i < height; ++i) {
			for (uint32_t j = 0; j < width; ++j) {
				if (blocks_[GetInd(j, i)] == WATER) {
					if (distance[i][j] <= WATER_SHALLOW_MAX_DISTANCE)
						blocks_[GetInd(j, i)] = WATER_SHALLOW;
					else if (distance[i][j] >= WATER_DEEP_MIN_DISTANCE)
						blocks_[GetInd(j, i)] = WATER_DEEP;
				}
			}
		}

		// -1.3. Remove deep water tiles with very low area
		const uint32_t DEEP_WATER_AREA_MIN = 6;
		vector<vector<bool>> visited(height, vector<bool>(width, false));
		for (uint32_t i = 0; i < height; ++i) {
			for (uint32_t j = 0; j < width; ++j) {
				if (blocks_[GetInd(j, i)] == WATER_DEEP && !visited[i][j]) {
					vector<Point> bfs;
					bfs.push_back({ j, i });
					for (size_t i = 0; i < bfs.size(); ++i) {
						Point current = bfs[i];
						for (Point p : grid_function::GetNeighbors(current, height, width)) {
							if (blocks_[GetInd(p.x, p.y)] == WATER_DEEP && !visited[p.y][p.x]) {
								bfs.push_back(p);
								visited[p.y][p.x] = true;
							}
						}
					}

					if (bfs.size() < DEEP_WATER_AREA_MIN) {
						for (Point point : bfs)
							blocks_[GetInd(point.x, point.y)] = WATER;
					}
				}
			}
		}

		// -1.3.2. Calculate area

	}
}
