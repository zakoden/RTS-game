#pragma once

#include <vector>
#include <unordered_set>

#include "behavior.h"

#include "../units/abstract_unit.h"
#include "../units/abstract_unit_factory.h"

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

	std::unordered_set<AbstractUnit*> targets_;
	size_t targets_num_ = 1;

	int radius_ = 200;

	AbstractUnitFactory* unit_factory_;
public:
	BehaviorTower(AbstractUnit* unit, AbstractUnitFactory* unit_factory);
	~BehaviorTower();

	void SetUnit(AbstractUnit* unit);
	// делает (-_-) действие
	void DoAction() override;
	void Attack(AbstractUnit* enemy);
	void FindTarget();

	void DeadCheck() override;
};
