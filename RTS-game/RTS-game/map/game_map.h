#pragma once

#include "map_layer.h"

class GameMap {
private:
	std::vector<MapLayer*> layers_;
	MapLayer* active_layer_;

public:
	GameMap(SDL_Renderer* renderer, uint32_t width, uint32_t height, size_t players_count, std::vector<MapLayer*> layers);
	~GameMap();										// Rule of three
	GameMap(const GameMap&) = delete;				// Rule of three
	GameMap& operator=(const GameMap&) = delete;	// Rule of three

	MapLayer* GetLayer(size_t layer_ind);
};
