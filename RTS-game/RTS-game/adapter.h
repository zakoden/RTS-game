#pragma once

#include "SDL.h"

#include "map/map_point.h"

namespace adapter {
	MapPoint UnitToMapCoordinates(int x, int y);

	MapPoint UnitToMapCoordinates(SDL_Point point);

	SDL_Point MapToUnitCoordinates(MapPoint point);
}