#pragma once

#include <iostream>
#include <vector>
#include <unordered_set>

#include "SDL.h"

#include "block_type.h"
#include "camera.h"
#include "point.h"
#include "../units/abstract_unit.h"

class GameMap {
private:
	const uint32_t BLOCK_SIZE = 8; // ðàçìåð áëîêà
	const uint32_t BLOCKS_IN_LINE = 10; // êîëè÷åñòâî áëîêîâ â ôàéëå ñ òåêñòóðîé

    uint32_t width_, height_; // øèðèíà, âûñîòà êàðòû
	std::vector<uint8_t> blocks_; // áëîêè êàðòû
	std::vector<std::unordered_set<AbstractUnit*>> units_in_block_; // þíèòû íà êàðòå
	SDL_Texture* tiles_ = nullptr; // òåêñòóðà ñî âñåìè áëîêàìè

	size_t GetInd(uint32_t x, uint32_t y) const;

public:

	GameMap(SDL_Renderer* renderer, uint32_t width, uint32_t height);
	~GameMap();
	uint32_t GetHeight();
	uint32_t GetWidth();
	uint32_t GetBlockSize();
	std::unordered_set<AbstractUnit*>* GetUnitsInBlock(uint32_t x, uint32_t y);

	bool IsPositionInMap(int x, int y);

	void AddUnit(AbstractUnit* unit, uint32_t x, uint32_t y);
	void DeleteUnit(AbstractUnit* unit, uint32_t x, uint32_t y);

	uint8_t GetBlock(uint32_t x, uint32_t y);
	void SetBlock(uint32_t x, uint32_t y, uint8_t value);
	void BlockDraw(SDL_Renderer* renderer, Camera* camera, uint32_t x, uint32_t y);
	void Draw(SDL_Renderer* renderer, Camera* camera); 

	uint8_t GetSubtype(BlockType type);
	void Generate();  // Generates random map
};