#include "adapter.h"

#include "map/game_map.h"

MapPoint adapter::UnitToMapCoordinates(int x, int y) {
    return { static_cast<uint32_t>(x) / GameMap::GetBlockSize(),
             static_cast<uint32_t>(y) / GameMap::GetBlockSize() };
}

MapPoint adapter::UnitToMapCoordinates(SDL_Point point) {
    return adapter::UnitToMapCoordinates(point.x, point.y);
}

SDL_Point adapter::MapToUnitCoordinates(MapPoint point) {
    return { static_cast<int>(point.x * GameMap::GetBlockSize()),
             static_cast<int>(point.y * GameMap::GetBlockSize()) };
}
