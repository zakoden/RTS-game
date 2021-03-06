#include "map_layer.h"

#include "../adapter.h"

#include "grid/grid_function.h"


size_t MapLayer::GetInd(uint32_t x, uint32_t y) const {
	return x + static_cast<size_t>(y) * width_;
}

MapLayer::~MapLayer() {

}

uint32_t MapLayer::GetHeight() {
	return height_;
}

uint32_t MapLayer::GetWidth() {
	return width_;
}

uint32_t MapLayer::GetBlockSize() {
	return BLOCK_SIZE;
}

const std::unordered_set<AbstractImmovableUnit*>& MapLayer::GetUnitsInBlock(uint32_t x, uint32_t y) const {
	return units_in_block_[GetInd(x, y)];
}

Grid<float>& MapLayer::GetDistanceToBase(uint8_t player_num) {
	return distance_to_base_[player_num];
}

bool MapLayer::IsBlockInMap(int x, int y) {
	return (x >= 0 && y >= 0 &&
		static_cast<uint32_t>(x) < width_ &&
		static_cast<uint32_t>(y) < height_);
}

bool MapLayer::IsPositionInMap(int x, int y) {
	return (x >= 0 && y >= 0 &&
		    static_cast<uint32_t>(x) < (BLOCK_SIZE * width_) && 
			static_cast<uint32_t>(y) < (BLOCK_SIZE * height_));
}

bool MapLayer::IsCellUncovered(MapPoint point, size_t player_num) const {
	return fog_of_war_[player_num][point];
}

SDL_Rect MapLayer::GetBlockRect(uint32_t x, uint32_t y) {
	SDL_Rect rect;
	rect.x = x * BLOCK_SIZE;
	rect.y = y * BLOCK_SIZE;
	rect.w = BLOCK_SIZE;
	rect.h = BLOCK_SIZE;
	return rect;
}

void MapLayer::AddUnit(AbstractImmovableUnit* unit, uint32_t x, uint32_t y) {
	units_in_block_[GetInd(x, y)].insert(unit);
}

void MapLayer::DeleteUnit(AbstractImmovableUnit* unit, uint32_t x, uint32_t y) {
	if (units_in_block_[GetInd(x, y)].find(unit) != units_in_block_[GetInd(x, y)].end()) {
		units_in_block_[GetInd(x, y)].erase(unit);
	}
}

uint8_t MapLayer::GetBlock(uint32_t x, uint32_t y, size_t player_num) const {
	return fog_of_war_[player_num][y][x] ? blocks_[GetInd(x, y)] : UNKNOWN;
}

void MapLayer::UncoverCell(int x, int y, size_t player_num) {
	fog_of_war_[player_num][adapter::UnitToMapCoordinates(x, y)] = true;
}

void MapLayer::SetBlock(uint32_t x, uint32_t y, uint8_t value) {
	blocks_[GetInd(x, y)] = value;
}

void MapLayer::DrawToTexture(SDL_Renderer* renderer) {
	SDL_SetRenderTarget(renderer, texture_save_);
	for (uint32_t y = 0; y < GetHeight(); ++y) {
		for (uint32_t x = 0; x < GetWidth(); ++x) {
			BlockDraw(renderer, x, y);
		}
	}
	SDL_SetRenderTarget(renderer, NULL);
}

void MapLayer::BlockDraw(SDL_Renderer* renderer, uint32_t x, uint32_t y, uint8_t block) {
	SDL_Texture* tiles_to_use = tiles_;
	if (block == UINT8_MAX || block == UINT8_MAX - 1) {
		block = blocks_[GetInd(x, y)];
	}

	if (rectangle_blocks_[block].x == -1) {
		rectangle_blocks_[block].x = (block % BLOCKS_IN_LINE) * BLOCK_SIZE;
		rectangle_blocks_[block].y = (block / BLOCKS_IN_LINE) * BLOCK_SIZE;
		rectangle_blocks_[block].w = BLOCK_SIZE;
		rectangle_blocks_[block].h = BLOCK_SIZE;
	}

	SDL_Rect to;
	to.x = x * BLOCK_SIZE;
	to.y = y * BLOCK_SIZE;
	to.w = BLOCK_SIZE;
	to.h = BLOCK_SIZE;
	SDL_RenderCopy(renderer, tiles_to_use, &rectangle_blocks_[block], &to);
}

void MapLayer::Draw(SDL_Renderer* renderer, Camera* camera, uint8_t player_num) {
	SDL_Rect to, from;
	from.x = 0;
	from.y = 0;
	from.w = texture_width_;
	from.h = texture_height_;
	to.x = -camera->GetCornerX(renderer);
	to.y = -camera->GetCornerY(renderer);
	to.w = texture_width_;
	to.h = texture_height_;
	SDL_RenderCopy(renderer, texture_save_, &from, &to);
}


void MapLayer::ApplyMask(SDL_Renderer* renderer, Camera* camera, int player_num, uint8_t block_num) {
	/*
	for (uint32_t y = 0; y < GetHeight(); ++y) {
		for (uint32_t x = 0; x < GetWidth(); ++x) {
			if (!IsCellUncovered({ x, y }, player_num)) {
				BlockDraw(renderer, camera, x, y, block_num);
			}
		}
	}
	*/
}

void MapLayer::AddBase(SDL_Point location, uint8_t player_num) {
	for (uint32_t y = 0; y < GetHeight(); ++y) {
		for (uint32_t x = 0; x < GetWidth(); ++x) {
			distance_to_base_[player_num][y][x] = std::min(
				distance_to_base_[player_num][y][x],
				grid_function::Distance({ x, y }, adapter::UnitToMapCoordinates(location)));
		}
	}
}

void MapLayer::FillMap(SDL_Renderer* renderer, uint32_t width, uint32_t height, size_t players_count) {
	size_t area = static_cast<size_t>(width) * height;
	width_ = width;
	height_ = height;
	blocks_ = std::vector<uint8_t>(area);
	units_in_block_ = std::vector<std::unordered_set<AbstractImmovableUnit*>>(area);
	fog_of_war_ = std::vector<Grid<char>>(
		players_count,
		Grid<char>(height, width, false)
		);
	distance_to_base_ = std::vector<Grid<float>>(
		players_count,
		Grid<float>(height, width, static_cast<float>(height + width))
		);

	// load texture
	SDL_Surface* surface = SDL_LoadBMP("pictures/map-tiles.bmp");
	tiles_ = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	surface = SDL_LoadBMP("pictures/map-tiles-fogged.bmp");
	fogged_tiles_ = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	texture_width_ = width_ * BLOCK_SIZE;
	texture_height_ = height_ * BLOCK_SIZE;
	texture_save_ = SDL_CreateTexture(
		renderer, SDL_PIXELFORMAT_UNKNOWN, SDL_TEXTUREACCESS_TARGET, texture_width_, texture_height_
	);
}
