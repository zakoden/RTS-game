#pragma once

class Behavior {
protected:
public:
	Behavior();
	~Behavior();

	virtual void AttackEnd();

	virtual void DoAction() = 0;

	virtual void DeadCheck();
};