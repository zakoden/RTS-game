#pragma once

#include "abstract_unit.h"

#include "../map/map_point.h"

class AbstractUnitFactory {
protected:
public:
	virtual AbstractUnit* CreateTest(size_t player, int x, int y) = 0;
	virtual AbstractUnit* CreateTest1(size_t player, int x, int y) = 0;
	virtual AbstractUnit* CreateTestHunter(size_t player, int x, int y) = 0;
	virtual AbstractUnit* CreateTestHunter2(size_t player, int x, int y) = 0;
	virtual AbstractUnit* CreateScout(size_t player, int x, int y, MapPoint target) = 0;

	virtual AbstractUnit* CreateBulletFire1(size_t player, int x, int y, int x_to, int y_to) = 0;

	virtual AbstractUnit* CreateBase(uint8_t player, int x, int y) = 0;
};