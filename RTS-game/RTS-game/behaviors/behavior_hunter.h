#pragma once

#include <vector>
#include <unordered_set>

#include "behavior.h"

#include "../units/abstract_unit.h"
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
	AbstractUnit* unit_;
	uint8_t cur_steps_[behavior_hunter::STEPS_CNT] = {};
	uint8_t max_steps_[behavior_hunter::STEPS_CNT];

	AbstractUnit* target_ = NULL;

	int distance_attack_ = 20;
	int radius_search_ = 200;

	AbstractUnitFactory* unit_factory_;
public:
	BehaviorHunter(AbstractUnit* unit, AbstractUnitFactory* unit_factory);
	~BehaviorHunter();

	void SetUnit(AbstractUnit* unit);

	void AttackEnd() override;
	void DoAction();
	void Attack(AbstractUnit* enemy);
	void FindTarget();

	void DeadCheck() override;
};