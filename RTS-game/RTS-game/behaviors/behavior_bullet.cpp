#include "behavior_bullet.h"

BehaviorBullet::BehaviorBullet(MovableUnit* unit, int x_to, int y_to) {
	unit_ = unit;
	dest_x_ = x_to + unit_->GetX() - unit_->GetCenterX();
	dest_y_ = y_to + unit_->GetY() - unit_->GetCenterY();
}

BehaviorBullet::~BehaviorBullet() {
}

void BehaviorBullet::SetUnit(MovableUnit* unit) {
	unit_ = unit;
}

void BehaviorBullet::DoAction() {
	int dx, dy;
	dx = dest_x_ - unit_->GetX();
	dy = dest_y_ - unit_->GetY();

	if (dx == 0 && dy == 0) {
		WithHitbox* target = unit_->GetEnemyInPoint(unit_->GetCenterX(), unit_->GetCenterY());
		if (target != NULL) {
			target->DamageApply(unit_->GetAttack());
		}
		unit_->Die();
	}

	if (dx * dx + dy * dy <= unit_->GetSpeed() * unit_->GetSpeed()) {
		unit_->SetPosition(dest_x_, dest_y_);
	} else {
		unit_->SetVector(dx, dy);
		unit_->VectorApplyBullet();
	}
}
