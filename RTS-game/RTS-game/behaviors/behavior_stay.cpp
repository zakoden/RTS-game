#include "behavior_stay.h"

BehaviorStay::BehaviorStay(MovableUnit* unit) {
	unit_ = unit;
}

BehaviorStay::~BehaviorStay() {

}

void BehaviorStay::SetUnit(MovableUnit* unit) {
	unit_ = unit;
}

void BehaviorStay::DoAction() {
}
