#include "map_point.h"

bool MapPoint::operator==(const MapPoint& other) const {
	return x == other.x && y == other.y;
}

bool MapPoint::operator!=(const MapPoint& other) const {
	return !operator==(other);
}
