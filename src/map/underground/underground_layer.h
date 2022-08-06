#pragma once

#include "../map/map_layer.h"

class UndergroundLayer : public MapLayer {
public:
	UndergroundLayer() = default;

	virtual void Generate(uint64_t seed) override;
};