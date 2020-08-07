#include "game_map.h"

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

bool GameMap::IsBlockInMap(int x, int y) {
	return (x >= 0 && y >= 0 &&
		static_cast<uint32_t>(x) < width_ &&
		static_cast<uint32_t>(y) < height_);
}

bool GameMap::IsPositionInMap(int x, int y) {
	return (x >= 0 && y >= 0 &&
		    static_cast<uint32_t>(x) < (BLOCK_SIZE * width_) && 
			static_cast<uint32_t>(y) < (BLOCK_SIZE * height_));
}

SDL_Rect GameMap::GetBlockRect(uint32_t x, uint32_t y) {
	SDL_Rect rect;
	rect.x = x * BLOCK_SIZE;
	rect.y = y * BLOCK_SIZE;
	rect.w = BLOCK_SIZE;
	rect.h = BLOCK_SIZE;
	return rect;
}

void GameMap::AddUnit(AbstractUnit* unit, uint32_t x, uint32_t y) {
	units_in_block_[GetInd(x, y)].insert(unit);
}

void GameMap::DeleteUnit(AbstractUnit* unit, uint32_t x, uint32_t y) {
	if (units_in_block_[GetInd(x, y)].find(unit) != units_in_block_[GetInd(x, y)].end()) {
		units_in_block_[GetInd(x, y)].erase(unit);
	}
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

uint8_t GameMap::GetBlock(uint32_t x, uint32_t y) {
	return blocks_[GetInd(x, y)];
}

void GameMap::SetBlock(uint32_t x, uint32_t y, uint8_t value) {
	blocks_[GetInd(x, y)] = value;
}

void GameMap::BlockDraw(SDL_Renderer* renderer, Camera* camera, uint32_t x, uint32_t y) {
    uint8_t block = blocks_[GetInd(x, y)];
	if (!units_in_block_[GetInd(x, y)].empty()) {
	//	block = 0;
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

double KekDist(int x1, int y1, int x2, int y2) {
	double ans;
	double dx = abs(x1 - x2);
	double dy = abs(y1 - y2);
	ans = sqrt(dx * dx + dy * dy);
	return ans;
}

void GameMap::GenerateHeightMap() {
	int chunck_size = 10;
	for (uint32_t y = 0; y < height_; y += chunck_size) {
		for (uint32_t x = 0; x < width_; x += chunck_size) {
			blocks_[GetInd(x, y)] = rand() % 256;
		}
	}
	for (uint32_t y = 0; y < height_; y += 1) {
		for (uint32_t x = 0; x < width_; x += 1) {
			if (x % chunck_size == 0 &&
				y % chunck_size == 0) continue;
			int val[4] = {0, 0, 0, 0};
			double dist[4] = {0.0, 0.0, 0.0, 0.0};
			int cx, cy;
			cx = x - x % chunck_size;
			cy = y - y % chunck_size;
			val[0] = blocks_[GetInd(cx, cy)];
			if ((cx + chunck_size) < width_) val[1] = blocks_[GetInd(cx + chunck_size, cy)];
			if ((cy + chunck_size) < height_) val[2] = blocks_[GetInd(cx, cy + chunck_size)];
			if ((cx + chunck_size) < width_ && 
				(cy + chunck_size) < height_) val[3] = blocks_[GetInd(cx+ chunck_size, cy + chunck_size)];
			dist[0] = KekDist(x, y, cx, cy);
			dist[1] = KekDist(x, y, cx + chunck_size, cy);
			dist[2] = KekDist(x, y, cx, cy + chunck_size);
			dist[3] = KekDist(x, y, cx + chunck_size, cy + chunck_size);
			double res = 0.0;
			double coef = 0.0;
			double sum = 0.0;
			for (int i = 0; i < 4; ++i) {
				coef = std::max(coef, dist[i]);
			}
			for (int i = 0; i < 4; ++i) {
				res += (double)val[i] * (10.0 / dist[i]);
				sum += (10.0 / dist[i]);
			}
			res /= sum;
			blocks_[GetInd(x, y)] = round(res);
		}
	}

	std::vector<BlockType> BLOCKS = {WATER_DEEP, WATER, WATER, WATER_SHALLOW, DESERT, GRASS_LIGHT, GRASS, GRASS_OTHER,
	MOUNTAIN_LOW, MOUNTAIN_HIGH};
	for (uint32_t y = 0; y < height_; y += 1) {
		for (uint32_t x = 0; x < width_; x += 1) {
			int len = 256 / BLOCKS.size();
			int val = blocks_[GetInd(x, y)] / len;
			if (val >= BLOCKS.size()) val = BLOCKS.size() - 1;
			blocks_[GetInd(x, y)] = GetSubtype(BLOCKS[val]);
		}
	}
}