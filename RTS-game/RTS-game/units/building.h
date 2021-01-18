#pragma once

#include "immovable_unit.h"

class Building : public ImmovableUnit {
public:
	Building(int attack, int defense, int max_health, TextureManager* texture_manager, GameMap* game_map, uint8_t cur_layer_ind);
	void Draw(SDL_Renderer* renderer, Camera* camera) override;
};