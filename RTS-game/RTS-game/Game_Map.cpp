#include "Game_Map.h"



GameMap::GameMap(SDL_Renderer* renderer, uint32_t width, uint32_t height) {
	width_ = width;
	height_ = height;
	blocks_.resize(width_ * height_);

	// load texture
	SDL_Surface* surface = SDL_LoadBMP("pictures/test.bmp");
	tiles_ = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
}

GameMap::~GameMap() {

}


uint8_t GameMap::GetBlock(uint32_t x, uint32_t y) {
	return blocks_[x + y * width_];
}

void GameMap::SetBlock(uint32_t x, uint32_t y, uint8_t value) {
	blocks_[x + y * width_] = value;
}

void GameMap::BlockDraw(SDL_Renderer* renderer, Camera* camera, uint32_t x, uint32_t y) {
	uint8_t block = blocks_[x + y * width_];
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



void GameMap::TestGenerate() {
	
	for (uint32_t y = 0; y < height_; ++y) {
		for (uint32_t x = 0; x < width_; ++x) {
			uint32_t ind = x + y * width_;
			blocks_[ind] = 0;
		}
	}
	blocks_[0 + 0 * width_] = rand() % 70;
	blocks_[width_ - 1 + 0 * width_] = rand() % 70;
	blocks_[0 + (height_ - 1) * width_] = rand() % 70;
	blocks_[width_ * height_ - 1] = rand() % 70;
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

