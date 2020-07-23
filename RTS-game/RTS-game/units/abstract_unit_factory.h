#pragma once

#include "unit.h"

class AbstractUnitFactory {
public:

	// Makes "Small fire" unit
	virtual Unit MakeFireSmall() = 0;
};