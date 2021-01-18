#pragma once

class Behavior {
protected:
public:
	Behavior();
	virtual ~Behavior();

	virtual void AttackEnd();

	virtual void DoAction() = 0;

	virtual void DeadCheck();
};