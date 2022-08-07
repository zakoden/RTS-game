#pragma once

#include <algorithm>
#include <iostream>
#include <vector>
#include <unordered_set>

#include "SDL.h"

#include "block_type.h"
#include "camera.h"
#include "grid/grid.h"
#include "map_point.h"
#include "../units/abstract_immovable_unit.h"

class MapLayer {
protected:
	const static uint32_t BLOCK_SIZE = 8; // ðàçìåð áëîêà
	const uint32_t BLOCKS_IN_LINE = 10; // êîëè÷åñòâî áëîêîâ â ôàéëå ñ òåêñòóðîé

    uint32_t width_, height_; // øèðèíà, âûñîòà êàðòû
	std::vector<uint8_t> blocks_; // áëîêè êàðòû

	std::vector<std::unordered_set<AbstractImmovableUnit*>> units_in_block_; // þíèòû íà êàðòå
	SDL_Texture* tiles_ = nullptr; // òåêñòóðà ñî âñåìè áëîêàìè
	SDL_Texture* fogged_tiles_ = nullptr; // Tiles in test-bw.bmp
		
	// Objects that refer to players, x[number] = object x for player #number
	vector<Grid<char>> fog_of_war_;  // [y][x] = 1 if this cell is uncovered, 0 otherwise
	vector<Grid<float>> distance_to_base_;  // [y][x] = distance to the nearest base

	// To reduce computation of rectangles
	std::vector<SDL_Rect> rectangle_blocks_ = vector<SDL_Rect>(256, { -1, -1, -1, -1 });

	SDL_Texture* texture_save_ = NULL;
	int texture_width_ = 0, texture_height_ = 0;

	size_t GetInd(uint32_t x, uint32_t y) const;
public:

	virtual ~MapLayer();
	uint32_t GetHeight();
	uint32_t GetWidth();
	static uint32_t GetBlockSize();
	const std::unordered_set<AbstractImmovableUnit*>& GetUnitsInBlock(uint32_t x, uint32_t y) const;
	Grid<float>& GetDistanceToBase(uint8_t player_num);

	bool IsBlockInMap(int x, int y);
	bool IsPositionInMap(int x, int y);
	
	// Returns fog_of_war_[player_num][y][x]
	bool IsCellUncovered(MapPoint point, size_t player_num) const;

	SDL_Rect GetBlockRect(uint32_t x, uint32_t y);

	void AddUnit(AbstractImmovableUnit* unit, uint32_t x, uint32_t y);
	void DeleteUnit(AbstractImmovableUnit* unit, uint32_t x, uint32_t y);

	// Gets a block in (x, y), UNKNOWN if player player_num doesn't know it yet
	uint8_t GetBlock(uint32_t x, uint32_t y, size_t player_num) const;

	// Uncovers cell in (x, y) (unit cell) for player player_num
	void UncoverCell(int x, int y, size_t player_num);

	void SetBlock(uint32_t x, uint32_t y, uint8_t value);
	void DrawToTexture(SDL_Renderer* renderer);
	void BlockDraw(SDL_Renderer* renderer, uint32_t x, uint32_t y, uint8_t block = UINT8_MAX);
	void Draw(SDL_Renderer* renderer, Camera* camera, uint8_t player_num = UINT8_MAX);

	// For user_manager, covers all covered cells with block_num
	void ApplyMask(SDL_Renderer* renderer, Camera* camera, int player_num, uint8_t block_num);

	void AddBase(SDL_Point location, uint8_t player_num);  // Processes the event that new base was added

	// DRY + Abstract class doesn't have a constructor
	void FillMap(SDL_Renderer* renderer, uint32_t width, uint32_t height, size_t players_count);

	/** 
	* Generates map based on a seed.
	* This function must not be called in constructor. It will be called in GameMap.
	* This function must be deterministic, i.e. if Generate is called with the same seed, then the results should be the same.
	* As the result, Generate must fill blocks_ array with blocks that will be used to generate map.
	*/
	virtual void Generate(uint64_t seed) = 0;
};