#include "behavior_hunter.h"

BehaviorHunter::BehaviorHunter(AbstractUnit* unit, AbstractUnitFactory* unit_factory) {
	unit_ = unit;
	unit_factory_ = unit_factory;
	max_steps_[behavior_hunter::Steps::FIND] = 30;
	cur_steps_[behavior_hunter::Steps::RELOAD] = rand() % 50;
	max_steps_[behavior_hunter::Steps::RELOAD] = 50;
}

BehaviorHunter::~BehaviorHunter() {
}

void BehaviorHunter::SetUnit(AbstractUnit* unit) {
	unit_ = unit;
}

void BehaviorHunter::AttackEnd() {
	target_->DamageApply(unit_->GetAttack());
	std::cout << "attack end" << std::endl;
}

void BehaviorHunter::DoAction() {
	for (size_t i = 0; i < behavior_hunter::Steps::STEPS_CNT; ++i) {
		cur_steps_[i]++;
		cur_steps_[i] %= max_steps_[i];
	}

	if (cur_steps_[behavior_hunter::Steps::FIND] == 0 && target_ == NULL) {
		FindTarget();
	}

	if (target_ != NULL) {
		Attack(target_);
	}
}

void BehaviorHunter::Attack(AbstractUnit* enemy) {
	if (unit_->HasEffect(Effect::ATTACKING)) {
		return;
	}
	int dx, dy;
	dx = enemy->GetCenterX() - unit_->GetCenterX();
	dy = enemy->GetCenterY() - unit_->GetCenterY();
	if ((dx * dx + dy * dy) > radius_attack_ * radius_attack_) {
		unit_->SetVector(dx, dy);
		unit_->VectorApply();
		return;
	}
	std::cout << "attack begin" << std::endl;
	unit_->SetVector(0.0, 0.0);
	unit_->AddEffect(Effect::ATTACKING);
}

void BehaviorHunter::FindTarget() {
	AbstractUnit* target = unit_->FindEnemyInRadius(radius_search_);
	if (target != NULL) {
		target_ = target;
	}
}

void BehaviorHunter::DeadCheck() {
	if (target_ == NULL) return;
	if (!target_->IsAlive()) {
		target_ = NULL;
	}
}
