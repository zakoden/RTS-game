#include "behavior_tower.h"

BehaviorTower::BehaviorTower(AbstractUnit* unit) {
	unit_ = unit;
	max_steps_[Steps::FIND] = 30;
	max_steps_[Steps::RELOAD] = 50;
}

BehaviorTower::~BehaviorTower() {

}

void BehaviorTower::SetUnit(AbstractUnit* unit) {
	unit_ = unit;
}

void BehaviorTower::DoAction() {
	for (size_t i = 0; i < Steps::STEPS_CNT; ++i) {
		cur_steps_[i]++;
		cur_steps_[i] %= max_steps_[i];
	}

	if (cur_steps_[Steps::FIND] == 0) {
		FindTarget();
	}

	if (cur_steps_[Steps::RELOAD] == 0) {
		for (size_t i = 0; i < targets_.size(); ++i) {
			Attack(targets_[i]);
		}
	}
	
}

void BehaviorTower::Attack(AbstractUnit* enemy) {

}

void BehaviorTower::FindTarget() {

}
