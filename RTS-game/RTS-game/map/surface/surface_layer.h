#pragma once

#include "../map_layer.h"

class SurfaceLayer : public MapLayer {
public:
	SurfaceLayer() = default;

	virtual void Generate(uint64_t seed) override;
};