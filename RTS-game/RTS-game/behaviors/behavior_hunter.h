#pragma once

#include <vector>
#include <unordered_set>

#include "behavior.h"

#include "../units/movable_unit.h"
#include "../units/abstract_unit_factory.h"

namespace behavior_hunter {
	enum Steps {
		COMMAND_UPDATE,
		RELOAD,
		FIND,
		STEPS_CNT
	};
}

class BehaviorHunter : public Behavior {
protected:
	MovableUnit* unit_;
	uint8_t cur_steps_[behavior_hunter::STEPS_CNT] = {};
	uint8_t max_steps_[behavior_hunter::STEPS_CNT];

	WithHitbox* target_ = NULL;

	int distance_attack_ = 20;
	int radius_search_ = 200;

	MovableUnitFactory* unit_factory_;
public:
	BehaviorHunter(MovableUnit* unit, MovableUnitFactory* unit_factory);
	~BehaviorHunter();

	void SetUnit(MovableUnit* unit);

	void AttackEnd() override;
	void DoAction() override;
	void Attack(WithHitbox* enemy);
	void FindTarget();

	void DeadCheck() override;
};