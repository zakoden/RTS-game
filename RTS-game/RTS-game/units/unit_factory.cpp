#include "unit_factory.h"

#include <iostream>

SDL_Texture* UnitFactory::GetTexture(const Unit& unit) {
	return units_textures_.count(unit.GetName()) ?
		units_textures_[unit.GetName()] : NULL;
}

SDL_Texture* UnitFactory::MakeTexture(const Unit& unit) {
	if (!units_textures_.count(unit.GetName()))
		units_textures_[unit.GetName()] = unit.Draw();
	return GetTexture(unit);
}

UnitFactory::UnitFactory(SDL_Renderer* renderer) : renderer_(renderer) {}

// attack, defense, max_health, speed, texture, name
Unit UnitFactory::MakeFireSmall() {
	const std::string SMALL_FIRE_NAME = "Small fire";
	Unit res(1, 0, 5, 1, units_textures_[SMALL_FIRE_NAME], SMALL_FIRE_NAME);
	res.AddEffect(ON_FIRE);
	return res;
}

UnitFactory::~UnitFactory() {
	for (auto name_texture : units_textures_)
		SDL_DestroyTexture(name_texture.second);
}
