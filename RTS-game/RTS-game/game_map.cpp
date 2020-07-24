#include "game_map.h"

#include <cassert>
#include <ctime>

#include <iostream>
#include <queue>
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

void GameMap::Generate() {
	// 1. Scattering random points
	const uint32_t CHUNK_SIZE = 128;
	const uint32_t CHUNKS_COUNT = (GetHeight() * GetWidth()) / CHUNK_SIZE;

	srand(static_cast<unsigned int>(time(0)));

	vector<vector<double>> distance(GetHeight(), vector<double>(GetWidth(), INT_MAX));
	vector<vector<int>> cluster(GetHeight(), vector<int>(GetWidth(), -1));
	for (uint32_t i = 0; i < CHUNKS_COUNT; ++i) {
		uint32_t x = rand() % GetWidth(), y = rand() % GetHeight();
		while (cluster[y][x] != -1)
			x = rand() % GetWidth(), y = rand() % GetHeight();
		cluster[y][x] = i;
		distance[y][x] = 0;
	}

	/* 2. Building voronoi diagram
		How? Let's do dijkstra algorithm from all current points,
		then for each cell c we'll find the closest point,
		and c will belong to closest point's cluster
		That will take O(area * log(area)) (area = height * width)
	*/
	std::priority_queue<std::pair<double, std::pair<uint32_t, uint32_t>>> dijkstra;
	
	// Inititalizing priority queue
	for (uint32_t i = 0; i < GetHeight(); ++i) {
		for (uint32_t j = 0; j < GetWidth(); ++j) {
			if (distance[i][j] == 0)
				dijkstra.push({ 0, {j, i}});
		}
	}

	while (!dijkstra.empty()) {
		auto pair = dijkstra.top();
		dijkstra.pop();
		uint32_t x = pair.second.first, y = pair.second.second;
		if (std::abs(-pair.first - distance[y][x]) < 1e-6) { // -pair.first == distance[y][x]
			// Going through all neighboring points
			for (int dx = -1; dx <= 1; ++dx) {
				if ((dx == -1 && x == 0) || (dx == 1 && x + 1 == GetWidth()))
					continue;

				for (int dy = -1; dy <= 1; ++dy) {
					if ((dy == -1 && y == 0) || (dy == 1 && y + 1 == GetHeight()))
						continue;
					
					double dist = std::sqrt(dx * dx + dy * dy);
					uint32_t to_x = x + dx, to_y = y + dy;  // Neighboring point
					if (distance[to_y][to_x] > distance[y][x] + dist) {
						distance[to_y][to_x] = distance[y][x] + dist;
						cluster[to_y][to_x] = cluster[y][x];
						dijkstra.push({ -distance[to_y][to_x], {to_y, to_x} });
					}
				}
			}
		}
	}
	/*
	for (uint32_t i = 0; i < GetHeight(); ++i)
		for (uint32_t j = 0; j < GetWidth(); ++j)
			assert(cluster[i][j] != -1);
			*/

	// 3. Give to each cluster random tile
	const vector<BlockType> allowed_blocks = { WATER, DESERT, GRASS_LIGHT, GRASS_DARK, GRASS };
	vector<BlockType> cluster_type{ CHUNKS_COUNT };
	for (uint32_t i = 0; i < CHUNKS_COUNT; ++i)
		cluster_type[i] = allowed_blocks[rand() % allowed_blocks.size()];

	// 4-?. WIP

	// ?. Give the result to blocks_ array
	for (uint32_t i = 0; i < GetHeight(); ++i) {
		for (uint32_t j = 0; j < GetWidth(); ++j) {
			blocks_[GetInd(j, i)] = cluster_type[cluster[j][i]];
		}
	}
}

/*
void GameMap::TestGenerate() {
	
	for (uint32_t y = 0; y < height_; ++y) {
		for (uint32_t x = 0; x < width_; ++x) {
			size_t ind = GetInd(x, y);
			blocks_[ind] = 0;
		}
	}

	blocks_[0 + 0 * width_] = rand() % 70;
	blocks_[GetInd(width_ - 1, 0) ] = rand() % 70;
	blocks_[0 + (height_ - 1) * width_] = rand() % 70;
	blocks_[static_cast<size_t>(width_) * height_ - 1] = rand() % 70;
	RecGenerate(0, 0, width_ - 1, height_ - 1);
	for (uint32_t i = 0; i < (width_ * height_); ++i) {
		blocks_[i] /= 10;
	}

}


void GameMap::RecGenerate(uint32_t l, uint32_t u, uint32_t r, uint32_t d) {
	if ((r - l) < 2 || (d - u) < 2) return;
	uint32_t cx, cy;
	cx = (l + r) / 2;
	cy = (d + u) / 2;
		blocks_[l + cy * width_] = (blocks_[l + u * width_] + blocks_[l + d * width_]) / 2;
		blocks_[r + cy * width_] = (blocks_[r + u * width_] + blocks_[r + d * width_]) / 2;
		blocks_[cx + u * width_] = (blocks_[l + u * width_] + blocks_[r + u * width_]) / 2;
		blocks_[cx + d * width_] = (blocks_[l + d * width_] + blocks_[r + d * width_]) / 2;

	if ((r - l) < 7 || (d - u) < 7) {
		blocks_[cx + cy * width_] = (blocks_[cx + u * width_] + blocks_[cx + d * width_] +
			                         blocks_[l + cy * width_] + blocks_[r + cy * width_]) / 4;
	} else {
		blocks_[cx + cy * width_] = rand() % 70;
	}
	RecGenerate(l, u, cx, cy);
	RecGenerate(cx, u, r, cy);
	RecGenerate(l, cy, cx, d);
	RecGenerate(cx, cy, r, d);
}
*/
