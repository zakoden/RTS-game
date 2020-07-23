#pragma once

#include <iostream>
#include <vector>
#include <unordered_set>

#include "SDL.h"

#include "camera.h"
#include "units/abstract_unit.h"

class GameMap {
private:
	const uint32_t BLOCK_SIZE = 8; // ������ �����
	const uint32_t BLOCKS_IN_LINE = 4; // ���������� ������ � ����� � ���������

    uint32_t width_, height_; // ������, ������ �����
	std::vector<uint8_t> blocks_; // ����� �����
	std::vector<std::unordered_set<AbstractUnit*>> units_in_block_; // ����� �� �����
	SDL_Texture* tiles_ = nullptr; // �������� �� ����� �������

	size_t GetInd(uint32_t x, uint32_t y);

public:

	GameMap(SDL_Renderer* renderer, uint32_t width, uint32_t height);
	~GameMap();
	uint32_t GetHeight();
	uint32_t GetWidth();

	void AddUnit(AbstractUnit* unit, uint32_t x, uint32_t y);
	void DeleteUnit(AbstractUnit* unit, uint32_t x, uint32_t y);

	uint8_t GetBlock(uint32_t x, uint32_t y);
	void SetBlock(uint32_t x, uint32_t y, uint8_t value);
	void BlockDraw(SDL_Renderer* renderer, Camera* camera, uint32_t x, uint32_t y);
	void Draw(SDL_Renderer* renderer, Camera* camera); 

	void TestGenerate();
	void RecGenerate(uint32_t l, uint32_t u, uint32_t r, uint32_t d);
};

