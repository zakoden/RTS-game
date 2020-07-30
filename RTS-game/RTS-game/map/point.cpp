#include "point.h"

bool Point::operator==(const Point& other) {
	return x == other.x && y == other.y;
}

bool Point::operator!=(const Point& other) {
	return !operator==(other);
}
