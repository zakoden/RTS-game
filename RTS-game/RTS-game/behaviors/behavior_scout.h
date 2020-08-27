#pragma once

#include "behavior.h"

#include "../clock.h"

#include "../map/game_map.h"
#include "../map/grid.h"
#include "../map/map_point.h"

#include "../units/abstract_unit.h"

class BehaviorScout : public Behavior {
private:
	AbstractUnit* unit_;
	GameMap* game_map_;
	SDL_Point target_ = { -1, -1 };
	MapPoint center_;

public:
	BehaviorScout(AbstractUnit* unit, GameMap* game_map, MapPoint center);

	virtual void DoAction() override;
};