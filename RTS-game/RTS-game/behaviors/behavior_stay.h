#pragma once

#include "iostream"

#include "behavior.h"

#include "../units/abstract_unit.h"

class BehaviorStay : public Behavior {
protected:
	AbstractUnit* unit_;
public:
	BehaviorStay(AbstractUnit* unit = NULL);
	~BehaviorStay();

	void SetUnit(AbstractUnit* unit); 
    // ������ (-_-) ��������
	void DoAction(); 
};
