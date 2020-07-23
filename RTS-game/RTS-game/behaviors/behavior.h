#pragma once

class Behavior {
protected:

public:
	Behavior();
	~Behavior();

	virtual void DoAction() = 0;
};