#pragma once

#include "unit.h"

class AbstractUnitFactory {
public:

	// ������ ����� "Small fire"
	virtual Unit MakeFireSmall() = 0;
};