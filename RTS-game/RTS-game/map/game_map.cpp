#include "game_map.h"

#include <time.h>

GameMap::GameMap(SDL_Renderer* renderer, uint32_t width, uint32_t height, size_t players_count, std::vector<MapLayer*> layers) 
	: layers_(layers)
{
	uint64_t seed = static_cast<uint64_t>(time(0));
	for (size_t i = 0; i < layers_.size(); ++i) {
		layers[i]->FillMap(renderer, width, height, players_count);
		layers[i]->Generate(seed);
	}
}

GameMap::~GameMap() {
	for (MapLayer* layer : layers_) {
		delete layer;
	}
}

size_t GameMap::GetLayerCount() {
	return layers_.size();
}

MapLayer* GameMap::GetLayer(uint8_t layer_ind) {
	return layers_[layer_ind];
}
