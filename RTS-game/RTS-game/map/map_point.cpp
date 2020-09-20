#include "map_point.h"

bool MapPoint::operator==(const MapPoint& other) {
	return x == other.x && y == other.y;
}

bool MapPoint::operator!=(const MapPoint& other) {
	return !operator==(other);
}
