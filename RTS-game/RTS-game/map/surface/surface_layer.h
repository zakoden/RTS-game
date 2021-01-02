#pragma once

#include "../map_layer.h"

class SurfaceLayer : public MapLayer {
public:
	SurfaceLayer(SDL_Renderer* renderer, uint32_t width, uint32_t height, size_t players_count);

	virtual void Generate() override;
};