#pragma once

#include <vector>
#include <unordered_set>

#include "behavior.h"

#include "../units/movable_unit.h"
#include "../units/abstract_unit_factory.h"

enum Steps {
	RELOAD,
	FIND,
	STEPS_CNT
};

class BehaviorTower : public Behavior {
protected:
	MovableUnit* unit_;
	uint8_t cur_steps_[STEPS_CNT] = {};
	uint8_t max_steps_[STEPS_CNT];

	std::unordered_set<MovableUnit*> targets_;
	size_t targets_num_ = 1;

	int radius_ = 200;

	MovableUnitFactory* unit_factory_;
public:
	BehaviorTower(MovableUnit* unit, MovableUnitFactory* unit_factory);
	~BehaviorTower();

	void SetUnit(MovableUnit* unit);
	// делает (-_-) действие
	void DoAction() override;
	void Attack(MovableUnit* enemy);
	void FindTarget();

	void DeadCheck() override;
};
