#pragma once

#include <vector>
#include <unordered_set>

#include "behavior.h"

#include "../units/abstract_unit.h"
#include "../units/abstract_unit_factory.h"

class BehaviorAroundPoint : public Behavior {
protected:
	AbstractUnit* unit_;

	Behavior* inner_behavior_;
	int center_x_, center_y_;
	int radius_;
	AbstractUnitFactory* unit_factory_;
public:
	BehaviorAroundPoint(AbstractUnit* unit, AbstractUnitFactory* unit_factory,
		                Behavior* inner_behavior, int center_x, int center_y, int radius);
	~BehaviorAroundPoint();

	void SetUnit(AbstractUnit* unit);

	void DoAction() override;

	void DeadCheck() override;
};