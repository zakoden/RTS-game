#pragma once

#include <array>
#include <string>
#include <unordered_map>

#include "SDL.h"

#include "unit.h"

class UnitFactory {
private:
	SDL_Renderer* renderer_;  // ��������

	// ����� �������� �������� ������ (��� �����������)
	std::unordered_map<std::string, SDL_Texture*> units_textures_;  

	// ������� ������ �������� �����
	SDL_Texture* GetTexture(const Unit& unit);

	// ������� ���� ����� � ������ ��� ��������
	SDL_Texture* MakeTexture(const Unit& unit);

public:
	// ������ ������� �� ���������
	explicit UnitFactory(SDL_Renderer* renderer);

	// ������ ����� "Small fire"
	Unit MakeFireSmall();
	
	// ������� �������, ������ ��� ��������
	~UnitFactory();
};