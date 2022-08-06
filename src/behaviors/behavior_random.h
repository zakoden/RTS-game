#pragma once

#include "iostream"

#include "behavior.h"

#include "../units/movable_unit.h"

class BehaviorRandom : public Behavior {
protected:
	MovableUnit* unit_;
	int cur_steps_ = 0;
	int steps_to_change_ = 50;
	double dx_ = 0.0, dy_ = 0.0;
public:
	BehaviorRandom(MovableUnit* unit = NULL);
	~BehaviorRandom();

	void SetUnit(MovableUnit* unit);
	// делает (-_-) действие
	void DoAction();
};
