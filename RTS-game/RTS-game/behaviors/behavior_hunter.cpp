#include "behavior_hunter.h"

BehaviorHunter::BehaviorHunter(MovableUnit* unit, MovableUnitFactory* unit_factory)
: unit_(unit)
, unit_factory_(unit_factory) {
	memset(cur_steps_, 0, sizeof cur_steps_);
	max_steps_[behavior_hunter::Steps::COMMAND_UPDATE] = 50;
	max_steps_[behavior_hunter::Steps::FIND] = 30;
	cur_steps_[behavior_hunter::Steps::RELOAD] = rand() % 50;
	max_steps_[behavior_hunter::Steps::RELOAD] = 50;
}

BehaviorHunter::~BehaviorHunter() {
}

void BehaviorHunter::SetUnit(MovableUnit* unit) {
	unit_ = unit;
}

void BehaviorHunter::AttackEnd() {
	if (target_ != NULL) target_->DamageApply(unit_->GetAttack());
}

void BehaviorHunter::DoAction() {
	for (size_t i = 0; i < behavior_hunter::Steps::STEPS_CNT; ++i) {
		cur_steps_[i] = (++cur_steps_[i]) % max_steps_[i];
	}

	if (unit_->HasEffect(Effect::ATTACKING)) {
		return;
	}

	if (unit_->HasEffect(Effect::HAS_COMMAND_POINT)) {
		if (cur_steps_[behavior_hunter::Steps::COMMAND_UPDATE] == 0) {
			target_ = NULL;
		}
	} else {
		if (cur_steps_[behavior_hunter::Steps::FIND] == 0 && target_ == NULL) {
			FindTarget();
		}
	}

	AbstractImmovableUnit* target = unit_->FindEnemyInRadius(distance_attack_);
	target_ = unit_->GetClosestUnit(target, target_);

	if (target_ != NULL) {
		Attack(target_);
	} else {
		if (unit_->HasEffect(Effect::HAS_COMMAND_POINT)) {
			int to_x, to_y;
			unit_->GetCommandPoint(to_x, to_y);
			double dx = (double)to_x - unit_->GetCenterX();
			double dy = (double)to_y - unit_->GetCenterY();
			unit_->SetVector(dx, dy);
			unit_->VectorApply();
		}
	}
}

void BehaviorHunter::Attack(AbstractImmovableUnit* enemy) {
	if (unit_->HasEffect(Effect::ATTACKING)) {
		return;
	}

	int dx = enemy->GetCenterX() - unit_->GetCenterX();
	int dy = enemy->GetCenterY() - unit_->GetCenterY();
	if ((dx * dx + dy * dy) > distance_attack_ * distance_attack_) {
		unit_->SetVector(dx, dy);
		unit_->VectorApply();
		return;
	}
	unit_->SetVector(0.0, 0.0);
	unit_->AddEffect(Effect::ATTACKING);
}

void BehaviorHunter::FindTarget() {
	AbstractImmovableUnit* target = unit_->FindEnemyInRadius(radius_search_);
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
