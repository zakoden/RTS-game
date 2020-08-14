#pragma once

#include <vector>
#include <unordered_set>

#include "behavior.h"

#include "../units/abstract_unit.h"
#include "../units/abstract_unit_factory.h"

namespace behavior_around_point {
	enum Steps {
		RELOAD,
		FIND,
		STEPS_CNT
	};
}

class BehaviorAroundPoint : public Behavior {
protected:
	AbstractUnit* unit_;
	uint8_t cur_steps_[behavior_around_point::STEPS_CNT] = {};
	uint8_t max_steps_[behavior_around_point::STEPS_CNT];

	AbstractUnit* target_ = NULL;

	int center_x_, center_y_;
	int radius_;
	AbstractUnitFactory* unit_factory_;
public:
	BehaviorAroundPoint(AbstractUnit* unit, AbstractUnitFactory* unit_factory);
	~BehaviorAroundPoint();

	void SetUnit(AbstractUnit* unit);

	void AttackEnd() override;
	void DoAction();
	void Attack(AbstractUnit* enemy);
	void FindTarget();

	void DeadCheck() override;
};