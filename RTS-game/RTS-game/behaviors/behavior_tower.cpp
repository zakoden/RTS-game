#include "behavior_tower.h"

BehaviorTower::BehaviorTower(AbstractUnit* unit, AbstractUnitFactory* unit_factory) {
	unit_ = unit;
	unit_factory_ = unit_factory;
	max_steps_[Steps::FIND] = 30;
	cur_steps_[Steps::RELOAD] = rand() % 50;
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
			Attack(*targets_.begin());
		}
	}
	
}

void BehaviorTower::Attack(AbstractUnit* enemy) {
	unit_factory_->CreateBulletFire1(unit_->GetPlayer(), 
		unit_->GetCenterX(), unit_->GetCenterY(),
		enemy->GetCenterX(), enemy->GetCenterY());
}

void BehaviorTower::FindTarget() {
	AbstractUnit* target = unit_->FindEnemyInRadius(radius_);
	if (target != NULL) {
		if (targets_.empty()) {
			targets_.insert(target);
		}
	}
}

void BehaviorTower::DeadCheck() {
	auto it = targets_.begin();
	while (it != targets_.end()) {
		if ((*it)->IsAlive()) {
			it++;
		} else {
			it = targets_.erase(it);
		}
	}
}
