#include "game_map.h"

GameMap::GameMap(SDL_Renderer* renderer, uint32_t width, uint32_t height, size_t players_count, std::vector<MapLayer*> layers) 
	: layers_(layers) 
	, active_layer_(layers.at(0))
{
	for (size_t i = 0; i < layers_.size(); ++i) {
		layers[i]->FillMap(renderer, width, height, players_count);
		layers[i]->Generate();
	}
}

GameMap::~GameMap() {
	for (MapLayer* layer : layers_) {
		delete layer;
	}
}

MapLayer* GameMap::GetLayer(size_t layer_ind) {
	return layers_[layer_ind];
}
