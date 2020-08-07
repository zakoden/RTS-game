#include "behavior_random.h"

BehaviorRandom::BehaviorRandom(AbstractUnit* unit) {
	unit_ = unit;
	cur_steps_ = rand() % steps_to_change_;
}

BehaviorRandom::~BehaviorRandom() {

}

void BehaviorRandom::SetUnit(AbstractUnit* unit) {
	unit_ = unit;
}

void BehaviorRandom::DoAction() {
	cur_steps_++;
	if (cur_steps_ == steps_to_change_) {
		int dx = -2 + rand() % 5;
		int dy = -2 + rand() % 5;
		if (unit_->GetX() < 0) dx = -dx;
		if (unit_->GetY() < 0) dy = -dy;
		dx_ = dx;
		dy_ = dy;
		cur_steps_ = 0;
	}
	unit_->SetVector(dx_, dy_);
	unit_->VectorApply();
}
