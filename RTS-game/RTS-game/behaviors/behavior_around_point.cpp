#include "behavior_around_point.h"

BehaviorAroundPoint::BehaviorAroundPoint(AbstractUnit* unit, AbstractUnitFactory* unit_factory) {
	unit_ = unit;
	unit_factory_ = unit_factory;
	max_steps_[behavior_around_point::Steps::FIND] = 30;
	cur_steps_[behavior_around_point::Steps::RELOAD] = rand() % 50;
	max_steps_[behavior_around_point::Steps::RELOAD] = 50;
}

BehaviorAroundPoint::~BehaviorAroundPoint() {
}

void BehaviorAroundPoint::SetUnit(AbstractUnit* unit) {
	unit_ = unit;
}

void BehaviorAroundPoint::AttackEnd() {
	target_->DamageApply(unit_->GetAttack());
}

void BehaviorAroundPoint::DoAction() {
	for (size_t i = 0; i < behavior_around_point::Steps::STEPS_CNT; ++i) {
		cur_steps_[i]++;
		cur_steps_[i] %= max_steps_[i];
	}

	if (cur_steps_[behavior_around_point::Steps::FIND] == 0 && target_ == NULL) {
		FindTarget();
	}

	if (target_ != NULL) {
		Attack(target_);
	}
}

void BehaviorAroundPoint::Attack(AbstractUnit* enemy) {
	if (unit_->HasEffect(Effect::ATTACKING)) {
		return;
	}
	int dx, dy;
	dx = enemy->GetCenterX() - unit_->GetCenterX();
	dy = enemy->GetCenterY() - unit_->GetCenterY();
	if ((dx * dx + dy * dy) > radius_ * radius_) {
		unit_->SetVector(dx, dy);
		unit_->VectorApply();
		return;
	}
	unit_->SetVector(0.0, 0.0);
	unit_->AddEffect(Effect::ATTACKING);
}

void BehaviorAroundPoint::FindTarget() {
	AbstractUnit* target = unit_->FindEnemyInRadius(radius_);
	if (target != NULL) {
		target_ = target;
	}
}

void BehaviorAroundPoint::DeadCheck() {
	if (target_ == NULL) return;
	if (!target_->IsAlive()) {
		target_ = NULL;
	}
}
