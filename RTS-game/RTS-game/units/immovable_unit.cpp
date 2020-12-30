#include "unit.h"

#include <algorithm>
#include <iostream>

// ----------protected-------------

void ImmovableUnit::InsertUnitToMap() {
	if (type_ == UnitType::Fly) return;
	int x, y;
	x = floor(x_);
	y = floor(y_);
	int x1, x2, y1, y2;
	x1 = (x + deltaX_) / game_map_->GetBlockSize();
	y1 = (y + deltaY_) / game_map_->GetBlockSize();
	x2 = (x + deltaX_ + width_ - 1) / game_map_->GetBlockSize();
	y2 = (y + deltaY_ + height_ - 1) / game_map_->GetBlockSize();

	for (int cur_y = y1; cur_y <= y2; ++cur_y) {
		for (int cur_x = x1; cur_x <= x2; ++cur_x) {
			game_map_->AddUnit(this, cur_x, cur_y);
		}
	}
}

void ImmovableUnit::DeleteUnitFromMap() {
	if (type_ == UnitType::Fly) return;
	int x, y;
	x = floor(x_);
	y = floor(y_);
	int x1, x2, y1, y2;
	x1 = (x + deltaX_) / game_map_->GetBlockSize();
	y1 = (y + deltaY_) / game_map_->GetBlockSize();
	x2 = (x + deltaX_ + width_ - 1) / game_map_->GetBlockSize();
	y2 = (y + deltaY_ + height_ - 1) / game_map_->GetBlockSize();

	for (int cur_y = y1; cur_y <= y2; ++cur_y) {
		for (int cur_x = x1; cur_x <= x2; ++cur_x) {
			game_map_->DeleteUnit(this, cur_x, cur_y);
		}
	}
}

void ImmovableUnit::UnitCollide(AbstractImmovableUnit* unit) {
	if (type_ == UnitType::Fly) return;
	double x1, x2, y1, y2;
	double cx1, cy1, cx2, cy2;
	double dx1, dy1, dx2, dy2;
	double width = 0.0, height = 0.0;

	this->GetVector(dx1, dy1);
	unit->GetVector(dx2, dy2);

	this->GetHitbox(x1, y1, x2, y2);
	width += x2 - x1;
	height += y2 - y1;
	cx1 = (x1 + x2) / 2.0;
	cy1 = (y1 + y2) / 2.0;

	unit->GetHitbox(x1, y1, x2, y2);
	width += x2 - x1;
	height += y2 - y1;
	cx2 = (x1 + x2) / 2.0;
	cy2 = (y1 + y2) / 2.0;

	width /= 2.0;
	height /= 2.0;
	height *= 0.7;

	if (abs(cx1 - cx2) < width && abs(cy1 - cy2) < height) {
		double vx, vy;
		vx = width - abs(cx1 - cx2);
		vy = height - abs(cy1 - cy2);
		if (abs(vx) < abs(vy)) {
			double to = cx2 - cx1;
			if (to * dx1 < 0.0) dx1 = 0.0;
			if (to * dx2 > 0.0) dx2 = 0.0;
			double sum_dx = dx1 + dx2;
			sum_dx *= 0.0;
			dx1 -= sum_dx;
			dx2 -= sum_dx;
			this->AddVector(-dx1, 0.0);
			unit->AddVector(-dx2, 0.0);
		}
		else {
			double to = cy2 - cy1;
			if (to * dy1 < 0.0) dy1 = 0.0;
			if (to * dy2 > 0.0) dy2 = 0.0;
			double sum_dy = dy1 + dy2;
			sum_dy *= 0.0;
			dy1 -= sum_dy;
			dy2 -= sum_dy;
			this->AddVector(0.0, -dy1);
			unit->AddVector(0.0, -dy2);
		}
	}

	if (abs(cx1 - cx2) < width * 0.8 && abs(cy1 - cy2) < height * 0.8) {
		double vx, vy;
		vx = width - abs(cx1 - cx2);
		vy = height - abs(cy1 - cy2);
		if (abs(vx) < abs(vy)) {
			double to = cx2 - cx1;
			this->AddVector(-to * 0.2, 0.0);
			unit->AddVector(to * 0.2, 0.0);
		}
		else {
			double to = cy2 - cy1;
			this->AddVector(0.0, -to * 0.2);
			unit->AddVector(0.0, to * 0.2);
		}
		return;
	}
}

bool ImmovableUnit::CanMoveOnBlock(uint32_t x, uint32_t y) {
	return true;
	//return (game_map_->GetBlock(x, y) != BlockType::WATER_SHALLOW);
}

// -----------public---------------

ImmovableUnit::~ImmovableUnit() {
	if (behavior_ != NULL) delete behavior_;
	if (type_ == UnitType::Ground) DeleteUnitFromMap();
}

int ImmovableUnit::GetX() { return x_; }

int ImmovableUnit::GetY() { return y_; }

int ImmovableUnit::GetCenterX() { return (int)x_ + deltaX_ + width_ / 2; }

int ImmovableUnit::GetCenterY() { return (int)y_ + deltaY_ + height_ / 2; }

int ImmovableUnit::GetAttack() { return attack_; }

void ImmovableUnit::GetHitbox(double& x1, double& y1, double& x2, double& y2) {
	x1 = x_ + deltaX_;
	y1 = y_ + deltaY_;
	x2 = x1 + width_;
	y2 = y1 + height_;
}

int ImmovableUnit::GetLegsY() {
	return y_ + texture_height_;
}

void ImmovableUnit::GetCollisionbox(double& x1, double& y1, double& x2, double& y2) {
	x1 = x_ + deltaX_;
	y1 = y_ + deltaY_ + 0.3 * height_;
	x2 = x1 + width_;
	y2 = y1 + height_;
}

void ImmovableUnit::AddVector(double, double) {}

void ImmovableUnit::SetPosition(int x, int y) {
	SetPosition((double)x, (double)y);
}

void ImmovableUnit::SetPosition(double x, double y) {
	x_ = x;
	y_ = y;
	if (type_ == UnitType::Ground) InsertUnitToMap();
}

void ImmovableUnit::GetVector(double& dx, double& dy) {
	dx = 0;
	dy = 0;
}

void ImmovableUnit::DamageApply(int damage) {
	int total_damage = std::clamp(damage - defense_, 0, damage);
	health_ -= total_damage;
	if (health_ < 0) {
		std::cout << "die, health : " << health_ << std::endl;
		Die();
	}
}

void ImmovableUnit::UncoverNearbyCells() {
	for (int dx = -scout_radius_; dx <= scout_radius_; dx += game_map_->GetBlockSize()) {
		for (int dy = -scout_radius_; dy <= scout_radius_; dy += game_map_->GetBlockSize()) {
			if (game_map_->IsPositionInMap(x_ + dx, y_ + dy) && (dx * dx + dy * dy) <= scout_radius_ * scout_radius_)
				game_map_->UncoverCell(x_ + dx, y_ + dy, player_);
		}
	}
}

void ImmovableUnit::AttackEnd() {
	RemoveEffect(Effect::ATTACKING);
	behavior_->AttackEnd();
}

void ImmovableUnit::DoAction() {
	int x = static_cast<int>(x_), y = static_cast<int>(y_);
	int x1 = (x + deltaX_) / game_map_->GetBlockSize();
	int y1 = (y + deltaY_) / game_map_->GetBlockSize();
	int x2 = (x + deltaX_ + width_ - 1) / game_map_->GetBlockSize();
	int y2 = (y + deltaY_ + height_ - 1) / game_map_->GetBlockSize();

	std::unordered_set<AbstractImmovableUnit*> used_units;
	for (int cur_y = y1; cur_y <= y2; ++cur_y) {
		for (int cur_x = x1; cur_x <= x2; ++cur_x) {
			for (AbstractImmovableUnit* unit : game_map_->GetUnitsInBlock(cur_x, cur_y)) {
				if (unit == this) continue;
				if (used_units.find(unit) != used_units.end()) continue;
				used_units.insert(unit);
				UnitCollide(unit);
			}
		}
	}
}

AbstractImmovableUnit* ImmovableUnit::GetClosestUnit(AbstractImmovableUnit* unit1, AbstractImmovableUnit* unit2) {
	if (unit1 == NULL)
		return unit2;
	if (unit2 == NULL)
		return unit1;
	uint64_t dist1 = 0, dist2 = 0;
	int64_t delt;
	delt = GetCenterX() - unit1->GetCenterX();
	dist1 += delt * delt;
	delt = GetCenterY() - unit1->GetCenterY();
	dist1 += delt * delt;

	delt = GetCenterX() - unit2->GetCenterX();
	dist2 += delt * delt;
	delt = GetCenterY() - unit2->GetCenterY();
	dist2 += delt * delt;
	if (dist1 < dist2) {
		return unit1;
	} else {
		return unit2;
	}
}

// to do : change this shit
AbstractImmovableUnit* ImmovableUnit::FindEnemyInRadius(int radius) {
	AbstractImmovableUnit* ans = NULL;
	int cx, cy; // unit center
	cx = x_ + deltaX_ + width_ / 2;
	cy = y_ + deltaY_ + height_ / 2;
	int block_size = game_map_->GetBlockSize();
	for (int y = cy - radius; y <= (cy + radius); y += block_size) {
		int dx = 0; 
		int dy;
		AbstractImmovableUnit* unit = NULL;
		do {
			unit = GetEnemyInPoint(cx + dx, y);
			ans = GetClosestUnit(ans, unit);
			unit = GetEnemyInPoint(cx - dx, y);
			ans = GetClosestUnit(ans, unit);
			dx += block_size;
			dy = y - cy;
		} while ((dx * dx) + (dy * dy) <= (radius * radius));
	}
	return ans;
}

AbstractImmovableUnit* ImmovableUnit::GetEnemyInPoint(int x, int y) {
	if (!game_map_->IsPositionInMap(x, y)) 
		return NULL;

	uint32_t x_block = x / game_map_->GetBlockSize();
	uint32_t y_block = y / game_map_->GetBlockSize();
	//game_map_->SetBlock(x_block, y_block, 1);
	for (AbstractImmovableUnit* unit : game_map_->GetUnitsInBlock(x_block, y_block)) {
		if (players_info_->CanAttack(player_, unit->GetPlayer())) {
			return unit;
		}
	}
	return NULL;
}

void ImmovableUnit::AddEffect(Effect effect) { 
	effects_.set(effect); 
}

void ImmovableUnit::RemoveEffect(Effect effect) { 
	effects_.reset(effect); 
}

bool ImmovableUnit::HasEffect(Effect effect) const { 
	return effects_.test(effect); 
}

void ImmovableUnit::SetType(UnitType type) { type_ = type; }

void ImmovableUnit::SetBehavior(Behavior* behavior) { behavior_ = behavior; }

void ImmovableUnit::SetPlayer(size_t player) { player_ = player; }

void ImmovableUnit::SetPlayersInfo(PlayersInfo* players_info) { players_info_ = players_info; }

size_t ImmovableUnit::GetPlayer() { return player_; }

void ImmovableUnit::DeadCheck() {
	if (behavior_ != NULL) {
		behavior_->DeadCheck();
	}
}

bool ImmovableUnit::IsAlive() { return alive_; }

void ImmovableUnit::Die() { alive_ = false; }
