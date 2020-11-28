#pragma once

#include <vector>
#include <unordered_set>

#include "behavior.h"

#include "../units/movable_unit.h"
#include "../units/abstract_unit_factory.h"

class BehaviorAroundPoint : public Behavior {
protected:
	MovableUnit* unit_;

	Behavior* inner_behavior_;
	int center_x_, center_y_;
	int radius_;
	MovableUnitFactory* unit_factory_;
public:
	BehaviorAroundPoint(MovableUnit* unit, MovableUnitFactory* unit_factory,
		Behavior* inner_behavior, int center_x, int center_y, int radius);
	~BehaviorAroundPoint();

	void SetUnit(MovableUnit* unit);

	void DoAction() override;

	void DeadCheck() override;
};