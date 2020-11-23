#pragma once

#include "movable_unit.h"
#include "building.h"
#include "entity.h"

#include "../map/map_point.h"

class MovableUnitFactory {
protected:
public:
	virtual MovableUnit* CreateTest(size_t player, int x, int y) = 0;
	virtual MovableUnit* CreateTest1(size_t player, int x, int y) = 0;
	virtual MovableUnit* CreateTestHunter(size_t player, int x, int y) = 0;
	virtual MovableUnit* CreateTestHunter2(size_t player, int x, int y) = 0;
	virtual MovableUnit* CreateScout(size_t player, int x, int y, MapPoint target) = 0;

	virtual MovableUnit* CreateFireSmallPoleax(size_t player, int x, int y) = 0;
	virtual MovableUnit* CreateFireSmallSpear(size_t player, int x, int y) = 0;
	virtual MovableUnit* CreateFireSmallLance(size_t player, int x, int y) = 0;

	virtual MovableUnit* CreateFireMediumPoleax(size_t player, int x, int y) = 0;

	virtual MovableUnit* CreateBulletFire1(size_t player, int x, int y, int x_to, int y_to) = 0;

	virtual MovableUnit* CreateBase(uint8_t player, int x, int y) = 0;

	virtual MovableUnit* CreateSmallHorizontalGrayWall(uint8_t player, int x, int y) = 0;
	virtual MovableUnit* CreateSmallVerticalGrayWall(uint8_t player, int x, int y) = 0;
	virtual MovableUnit* CreateSmallGrayTower(uint8_t player, int x, int y) = 0;

	virtual Entity* CreateBamboo(int x, int y) = 0;
};