#pragma once

#include "iostream"

#include "behavior.h"

#include "../units/movable_unit.h"

class BehaviorStay : public Behavior {
protected:
	MovableUnit* unit_;
public:
	BehaviorStay(MovableUnit* unit = NULL);
	~BehaviorStay();

	void SetUnit(MovableUnit* unit); 
    // делает (-_-) действие
	void DoAction(); 
};
