#include "behavior_stay.h"

BehaviorStay::BehaviorStay(AbstractUnit* unit) {
	unit_ = unit;
}

BehaviorStay::~BehaviorStay() {

}

void BehaviorStay::SetUnit(AbstractUnit* unit) {
	unit_ = unit;
}

void BehaviorStay::DoAction() {
}
