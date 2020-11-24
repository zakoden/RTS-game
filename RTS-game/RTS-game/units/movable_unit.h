#pragma once

#include "immovable_unit.h"

#include <bitset>
#include <cmath>
#include <memory>

#include <iostream>

#include "SDL.h"

#include "unit_type.h"

#include "../map/camera.h"

#include "../behaviors/behavior.h"
#include "../behaviors/status_effects.h"

#include "../player/players_info.h"

class MovableUnit : public ImmovableUnit {
public:
	virtual ~MovableUnit() = default;
	virtual int GetSpeed() = 0;

	virtual void SetPosition(int x, int y) = 0;
	virtual void SetPosition(double x, double y) = 0;
	virtual void SetVector(int dx, int dy) = 0;
	virtual void SetVector(double dx, double dy) = 0;
	virtual void AddVector(double dx, double dy) = 0;
	virtual void VectorApply() = 0;
	virtual void VectorApplyBullet() = 0;
	virtual void SetCommandPoint(int x, int y) = 0;
	virtual void GetCommandPoint(int& x, int& y) = 0;

	virtual void Move() = 0;
};