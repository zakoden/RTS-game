#pragma once

#include "../behaviors/behavior.h"

class Actionable {
public:
	virtual ~Actionable() = default;

	virtual int GetAttack() = 0;

	virtual void AttackEnd() = 0;

	virtual void DoAction() = 0;
	virtual void SetBehavior(Behavior* behavior) = 0;
};