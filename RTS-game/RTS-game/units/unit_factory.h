#pragma once

#include "abstract_unit_factory.h"

#include <array>
#include <string>
#include <unordered_map>

#include "SDL.h"

#include "unit.h"

class UnitFactory : public AbstractUnitFactory {
private:
	SDL_Renderer* renderer_;

	// Textures of units are kept there, so we don't have to download multiple textures
	std::unordered_map<std::string, SDL_Texture*> units_textures_;  

	// Gets texture from an unit
	SDL_Texture* GetTexture(const Unit& unit);

	// Makes texture for an unit
	SDL_Texture* MakeTexture(const Unit& unit);

public:
	explicit UnitFactory(SDL_Renderer* renderer);

	// Makes "Small fire" unit
	Unit MakeFireSmall() override;
	
	// Destroys factory, and destroys all textures in units_textures_
	~UnitFactory();
};