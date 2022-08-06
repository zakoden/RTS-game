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

#include "movable.h"

class MovableUnit : public ImmovableUnit, public Movable {
public:
	virtual ~MovableUnit() = default;
};