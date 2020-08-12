#include "point.h"

struct Triple {
	float distance; Point point;
	inline bool operator<(const Triple& other) const { return distance > other.distance; }
};