#pragma once

#include <vector>

#include "behavior.h"

#include "../units/abstract_unit.h"

enum Steps {
	RELOAD,
	FIND,
	STEPS_CNT
};

class BehaviorTower : public Behavior {
protected:
	AbstractUnit* unit_;
	uint8_t cur_steps_[STEPS_CNT] = {};
	uint8_t max_steps_[STEPS_CNT];

	std::vector<AbstractUnit*> targets_;
	size_t targets_num_ = 1;

	int radius_ = 100;
public:
	BehaviorTower(AbstractUnit* unit = NULL);
	~BehaviorTower();

	void SetUnit(AbstractUnit* unit);
	// делает (-_-) действие
	void DoAction();
	void Attack(AbstractUnit* enemy);
	void FindTarget();
};
