#include "map_point.h"

struct Triple {
	float distance; MapPoint point;
	inline bool operator<(const Triple& other) const { return distance > other.distance; }
};