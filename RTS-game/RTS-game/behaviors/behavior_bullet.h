#pragma once

#include "behavior.h"

#include "../units/movable_unit.h"

class BehaviorBullet : public Behavior {
protected:
	MovableUnit* unit_;

	int damage_ = 10;
	int dest_x_, dest_y_;

public:
	BehaviorBullet(MovableUnit* unit, int x_to, int y_to);
	~BehaviorBullet();

	void SetUnit(MovableUnit* unit);

	void DoAction();
};
