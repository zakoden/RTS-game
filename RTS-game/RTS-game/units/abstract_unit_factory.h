#pragma once

#include "abstract_unit.h"

class AbstractUnitFactory {
protected:
public:
	virtual AbstractUnit* CreateTest(size_t player, int x, int y) = 0;
	virtual AbstractUnit* CreateTest1(size_t player, int x, int y) = 0;

	virtual AbstractUnit* CreateBulletFire1(size_t player, int x, int y) = 0;
};