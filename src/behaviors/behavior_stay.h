#pragma once

#include "iostream"

#include "behavior.h"

#include "../units/movable_unit.h"

class BehaviorStay : public Behavior {
protected:
	AbstractImmovableUnit* unit_;
public:
	BehaviorStay(AbstractImmovableUnit* unit = NULL);
	~BehaviorStay();

	void SetUnit(AbstractImmovableUnit* unit);
	// делает (-_-) действие
	void DoAction();
};
