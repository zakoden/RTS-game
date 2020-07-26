#include "game_map.h"

#include <cassert>
#include <ctime>

#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>

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

void GameMap::AddUnit(AbstractUnit* unit, uint32_t x, uint32_t y) {
	units_in_block_[GetInd(x, y)].insert(unit);
}

void GameMap::DeleteUnit(AbstractUnit* unit, uint32_t x, uint32_t y) {
	if (units_in_block_[GetInd(x, y)].find(unit) != units_in_block_[GetInd(x, y)].end()) {
		units_in_block_[GetInd(x, y)].erase(unit);
	}
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

uint32_t GameMap::SquaredDistance(const SDL_Point& a, const SDL_Point& b) const {
	return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}

// Generates random map
void GameMap::Generate() {
	uint32_t height = GetHeight(), width = GetWidth();
	// 1. Scattering random points (that will be centers of clusters)
	const uint32_t CHUNK_SIZE = 100;
	const uint32_t CHUNKS_COUNT = (height * width) / CHUNK_SIZE;

	srand(static_cast<unsigned int>(time(0)));

	float max_distance = height + width;  // A suboptimal distance between opposing corners

	// Distance between some point and closest cluster point
	vector<vector<float>> distance(height, vector<float>(width, max_distance));

	// To which cluster this point belongs (-1 for "no cluster")
	vector<vector<int>> cluster(height, vector<int>(width, -1));

	vector<SDL_Point> centers(CHUNKS_COUNT);

	for (uint32_t i = 0; i < CHUNKS_COUNT; ++i) {
		int x = rand() % width, y = rand() % height;  // The random point
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
	{
		std::priority_queue<std::pair<float, std::pair<uint32_t, uint32_t>>> dijkstra;
		// Elements in dijkstra are in form {-distance, {x, y} (point)}

		const float EPS = 1e-3;

		// Inititalizing priority queue
		for (uint32_t i = 0; i < height; ++i)
			for (uint32_t j = 0; j < width; ++j)
				if (distance[i][j] < EPS)
					dijkstra.push({ 0, {j, i} });

		while (!dijkstra.empty()) {
			auto pair = dijkstra.top();
			dijkstra.pop();
			uint32_t x = pair.second.first, y = pair.second.second;
			if (std::abs(-pair.first - distance[y][x]) > EPS)  // -pair.first != distance[y][x]
				continue;

			// Going through all neighboring points
			for (int dx = -1; dx <= 1; ++dx) {
				if ((dx == -1 && x == 0) || (dx == 1 && x + 1 == width))  // x is out of bounds
					continue;

				for (int dy = -1; dy <= 1; ++dy) {
					if ((dy == -1 && y == 0) || (dy == 1 && y + 1 == height))  // y is out of bounds
						continue;

					float dist = std::sqrtf(static_cast<float>(dx * dx + dy * dy));
					uint32_t to_x = x + dx, to_y = y + dy;  // Neighboring point
					if (distance[to_y][to_x] > distance[y][x] + dist) {
						distance[to_y][to_x] = distance[y][x] + dist;
						cluster[to_y][to_x] = cluster[y][x];
						dijkstra.push({ -distance[to_y][to_x], {to_x, to_y} });
					}
				}
			}
		}
	}

	// 3. Give to each cluster random tile
	const vector<BlockType> allowed_blocks = { WATER, DESERT, GRASS_LIGHT, GRASS_DARK, GRASS };
	vector<BlockType> cluster_type{ CHUNKS_COUNT };
	for (uint32_t i = 0; i < CHUNKS_COUNT; ++i)
		cluster_type[i] = allowed_blocks[rand() % allowed_blocks.size()];

	// 4. Unite all small clusters with big ones
	{
		// 4.1. Find area of clusters
		vector<std::pair<uint32_t, uint32_t>> area{ CHUNKS_COUNT };
		for (size_t i = 0; i < area.size(); ++i)
			area[i] = { 0, i };
		for (uint32_t i = 0; i < height; ++i)
			for (uint32_t j = 0; j < width; ++j)
				++area[cluster[i][j]].first;

		// 4.2 Remove chunks with area < AREA_MIN
		sort(area.begin(), area.end());
		std::unordered_map<uint32_t, uint32_t> chunks_to_remove;
		const size_t AREA_MIN = 80;
		for (size_t i = 0; i < area.size() && area[i].first < AREA_MIN; ++i)
			chunks_to_remove.insert({ area[i].second, 0 });
		std::cerr << chunks_to_remove.size() << std::endl;

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

	// 5. Give the result to blocks_ array
	for (uint32_t i = 0; i < height; ++i) {
		for (uint32_t j = 0; j < width; ++j) {
			blocks_[GetInd(j, i)] = cluster_type[cluster[j][i]];
		}
	}

	// 6. Giving subtypes to tiles
	for (uint32_t i = 0; i < height; ++i) {
		for (uint32_t j = 0; j < width; ++j) {
			blocks_[GetInd(j, i)] = GetSubtype(blocks_[GetInd(j, i)], j, i);
		}
	}
}
