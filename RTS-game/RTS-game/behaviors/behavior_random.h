#pragma once

#include "iostream"

#include "behavior.h"

#include "../units/abstract_unit.h"

class BehaviorRandom : public Behavior {
protected:
	AbstractUnit* unit_;
	int cur_steps_ = 0;
	int steps_to_change_ = 50;
	double dx_ = 0.0, dy_ = 0.0;
public:
	BehaviorRandom(AbstractUnit* unit = NULL);
	~BehaviorRandom();

	void SetUnit(AbstractUnit* unit);
	// делает (-_-) действие
	void DoAction();
};
