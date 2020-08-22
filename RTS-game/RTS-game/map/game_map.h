#pragma once

#include <algorithm>
#include <iostream>
#include <vector>
#include <unordered_set>

#include "SDL.h"

#include "block_type.h"
#include "camera.h"
#include "grid.h"
#include "point.h"
#include "../units/abstract_unit.h"

class GameMap {
private:
	const static uint32_t BLOCK_SIZE = 8; // ðàçìåð áëîêà
	const uint32_t BLOCKS_IN_LINE = 10; // êîëè÷åñòâî áëîêîâ â ôàéëå ñ òåêñòóðîé

    uint32_t width_, height_; // øèðèíà, âûñîòà êàðòû
	std::vector<uint8_t> blocks_; // áëîêè êàðòû
	std::vector<std::unordered_set<AbstractUnit*>> units_in_block_; // þíèòû íà êàðòå
	SDL_Texture* tiles_ = nullptr; // òåêñòóðà ñî âñåìè áëîêàìè

	// fog_of_war_[player_number][y][x] = 1 if this cell is uncovered, 0 otherwise
	vector<Grid<char>> fog_of_war_;

	std::vector<SDL_Rect> rectangle_blocks_ = vector<SDL_Rect>(256, { -1, -1, -1, -1 });

	size_t GetInd(uint32_t x, uint32_t y) const;
public:

	GameMap(SDL_Renderer* renderer, uint32_t width, uint32_t height, size_t players_count);
	~GameMap();
	uint32_t GetHeight();
	uint32_t GetWidth();
	static uint32_t GetBlockSize();
	std::unordered_set<AbstractUnit*>* GetUnitsInBlock(uint32_t x, uint32_t y);

	bool IsBlockInMap(int x, int y);
	bool IsPositionInMap(int x, int y);
	
	// Returns fog_of_war_[player_num][y][x]
	bool IsCellUncovered(uint32_t x, uint32_t y, size_t player_num) const;

	SDL_Rect GetBlockRect(uint32_t x, uint32_t y);

	void AddUnit(AbstractUnit* unit, uint32_t x, uint32_t y);
	void DeleteUnit(AbstractUnit* unit, uint32_t x, uint32_t y);

	// Gets a block in (x, y), UNKNOWN if player player_num doesn't know it yet
	uint8_t GetBlock(uint32_t x, uint32_t y, size_t player_num) const;

	// Uncovers cell in (x, y) for player player_num
	void UncoverCell(uint32_t x, uint32_t y, size_t player_num);

	void SetBlock(uint32_t x, uint32_t y, uint8_t value);
	void BlockDraw(SDL_Renderer* renderer, Camera* camera, uint32_t x, uint32_t y, uint8_t block = UINT8_MAX);
	void Draw(SDL_Renderer* renderer, Camera* camera, int player_num = -1); 
	void ApplyMask(SDL_Renderer* renderer, Camera* camera, int player_num, uint8_t block_num);

	Grid<float> GenerateHeights();  // Generates a map of heights
	void Generate();  // Generates random map
};