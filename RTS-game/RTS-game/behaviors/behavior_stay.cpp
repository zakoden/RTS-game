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
	if (rand() % 300 == 0) std::cout << "I stay" << std::endl;
	if (rand() % 300 == 0) std::cout << "move me!!!" << std::endl;
	if (rand() % 300 == 0) std::cout << "kek ahah" << std::endl;
}
