#pragma once

#include "unit.h"

class AbstractUnitFactory {
public:

	// Создаёт юнита "Small fire"
	virtual Unit MakeFireSmall() = 0;
};