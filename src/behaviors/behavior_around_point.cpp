#include "behavior_around_point.h"

BehaviorAroundPoint::BehaviorAroundPoint(MovableUnit* unit, MovableUnitFactory* unit_factory,
	                                     Behavior* inner_behavior, int center_x, int center_y, int radius) {
	unit_ = unit;
	unit_factory_ = unit_factory;
	inner_behavior_ = inner_behavior;
	center_x_ = center_x;
	center_y_ = center_y;
	radius_ = radius;
}

BehaviorAroundPoint::~BehaviorAroundPoint() {
}

void BehaviorAroundPoint::SetUnit(MovableUnit* unit) {
	unit_ = unit;
}

void BehaviorAroundPoint::DoAction() {
	inner_behavior_->DoAction();
	double dx, dy;
	unit_->GetVector(dx, dy);
	dx += (double)unit_->GetCenterX() - (double)center_x_;
	dy += (double)unit_->GetCenterY() - (double)center_y_;
	if (dx * dx + dy * dy > (double)radius_ * radius_) {
		unit_->SetVector(0.0, 0.0);
	}
}

void BehaviorAroundPoint::DeadCheck() {
}
