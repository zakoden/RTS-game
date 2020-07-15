#pragma once

#include <array>
#include <string>
#include <unordered_map>

#include "SDL.h"

#include "unit.h"

class UnitFactory {
private:
	SDL_Renderer* renderer_;  // Рендерер

	// Здесь хранятся текстуры юнитов (для оптимизации)
	std::unordered_map<std::string, SDL_Texture*> units_textures_;  

	// Функция достаёт текстуру юнита
	SDL_Texture* GetTexture(const Unit& unit);

	// Функция берёт юнита и рисует ему текстуру
	SDL_Texture* MakeTexture(const Unit& unit);

public:
	// Строит фабрику по рендереру
	explicit UnitFactory(SDL_Renderer* renderer);

	// Создаёт юнита "Small fire"
	Unit MakeFireSmall();
	
	// Удаляет фабрику, удаляя все текстуры
	~UnitFactory();
};