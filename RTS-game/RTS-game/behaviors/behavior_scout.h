#pragma once

#include "behavior.h"

#include "../clock.h"

#include "../map/map_layer.h"
#include "../map/grid/grid.h"
#include "../map/map_point.h"

#include "../units/movable_unit.h"

class BehaviorScout : public Behavior {
private:
	MovableUnit* unit_;
	MapLayer* game_map_;
	SDL_Point target_ = { -1, -1 };
	MapPoint center_;

public:
	BehaviorScout(MovableUnit* unit, MapLayer* game_map, MapPoint center);

	virtual void DoAction() override;
};