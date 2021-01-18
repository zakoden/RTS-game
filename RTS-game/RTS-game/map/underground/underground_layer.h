#pragma once

#include "../map_layer.h"

class UndergroundLayer : public MapLayer {
public:
	UndergroundLayer() = default;

	virtual void Generate(uint64_t seed) override;
};