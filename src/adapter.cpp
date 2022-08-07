#include "adapter.h"

#include "map/map_layer.h"

MapPoint adapter::UnitToMapCoordinates(int x, int y) {
    return { static_cast<uint32_t>(x) / MapLayer::GetBlockSize(),
             static_cast<uint32_t>(y) / MapLayer::GetBlockSize() };
}

MapPoint adapter::UnitToMapCoordinates(SDL_Point point) {
    return adapter::UnitToMapCoordinates(point.x, point.y);
}

SDL_Point adapter::MapToUnitCoordinates(MapPoint point) {
    return { static_cast<int>(point.x * MapLayer::GetBlockSize()),
             static_cast<int>(point.y * MapLayer::GetBlockSize()) };
}
