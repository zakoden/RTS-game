#pragma once

#include "behavior.h"

#include "../units/abstract_unit.h"

class BehaviorBullet : public Behavior {
protected:
	AbstractUnit* unit_;

	int damage_ = 10;
	int dest_x_, dest_y_;

public:
	BehaviorBullet(AbstractUnit* unit, int x_to, int y_to);
	~BehaviorBullet();

	void SetUnit(AbstractUnit* unit);

	void DoAction();
};
