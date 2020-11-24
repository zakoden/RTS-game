#include "behavior_stay.h"

BehaviorStay::BehaviorStay(AbstractImmovableUnit* unit) {
	unit_ = unit;
}

BehaviorStay::~BehaviorStay() {

}

void BehaviorStay::SetUnit(AbstractImmovableUnit* unit) {
	unit_ = unit;
}

void BehaviorStay::DoAction() {
}
