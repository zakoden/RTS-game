#include "unit.h"

#include <iostream>

// ----------protected-------------

void Unit::InsertUnitToMap() {
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

void Unit::DeleteUnitFromMap() {
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

bool Unit::CanMoveOnBlock(uint32_t x, uint32_t y) {
	return (game_map_->GetBlock(x, y) != BlockType::WATER_SHALLOW);
}

// -----------public---------------

Unit::Unit(int attack, int defense, int max_health, double speed,
	       TextureManager* texture_manager, GameMap* game_map) {
	attack_ = attack;
	defense_ = defense;
	health_ = max_health;
	max_health_ = max_health;
	speed_ = speed;
	texture_manager_ = texture_manager;
	game_map_ = game_map;

	behavior_ = NULL;
}

Unit::~Unit() {
	if (behavior_ != NULL) delete behavior_;
	if (type_ == UnitType::Ground) DeleteUnitFromMap();
}

int Unit::GetX() {
	return x_;
}

int Unit::GetY() {
	return y_;
}

int Unit::GetCenterX() {
	return (int)x_ + deltaX_ + width_ / 2;
}

int Unit::GetCenterY() {
	return (int)y_ + deltaY_ + height_ / 2;
}

int Unit::GetSpeed() {
	return speed_;
}

int Unit::GetAttack() {
	return attack_;
}

void Unit::GetHitbox(double& x1, double& y1, double& x2, double& y2) {
	x1 = x_ + deltaX_;
	y1 = y_ + deltaY_;
	x2 = x1 + width_;
	y2 = y1 + height_;
}

void Unit::GetVector(double& dx, double& dy) {
	dx = dx_;
	dy = dy_;
}


void Unit::SetPosition(int x, int y) {
	SetPosition((double)x, (double)y);
}

void Unit::SetPosition(double x, double y) {
	x_ = x;
	y_ = y;
	if (type_ == UnitType::Ground) InsertUnitToMap();
}

void Unit::SetVector(int dx, int dy) {
	dx_ = (double)dx;
	dy_ = (double)dy;
}

void Unit::SetVector(double dx, double dy) {
	dx_ = dx;
	dy_ = dy;
}

void Unit::AddVector(double dx, double dy) {
	dx_ += dx;
	dy_ += dy;
}

void Unit::VectorApply() {
	double EPS = 1e-5;
	if (abs(dx_) < EPS && abs(dy_) < EPS) return;
	double v_len = std::sqrt(dx_ * dx_ + dy_ * dy_);
	dx_ = (dx_ * speed_) / v_len;
	dy_ = (dy_ * speed_) / v_len;
}

void Unit::VectorApplyBullet() {
	double EPS = 1e-5;
	if (abs(dx_) < EPS && abs(dy_) < EPS) return;
	double v_len = std::sqrt(dx_ * dx_ + dy_ * dy_);
	dx_ = (dx_ * speed_) / v_len;
	dy_ = (dy_ * speed_) / v_len;
	x_ += dx_;
	y_ += dy_;
	if (x_ < 0) {
		x_ = 0;
	}
	if (y_ < 0) {
		y_ = 0;
	}
	if (x_ > (8 * game_map_->GetWidth() - width_)) {
		x_ = (8 * game_map_->GetWidth() - width_);
	}
	if (y_ > (8 * game_map_->GetHeight() - height_)) {
		y_ = (8 * game_map_->GetHeight() - height_);
	}
}

void Unit::DamageApply(int damage) {
	int total_damage = damage - defense_;
	if (total_damage < 0) total_damage = 0;
	health_ -= total_damage;
	if (health_ < 0) {
		std::cout << "die, health : " << health_ << std::endl;
		Die();
	}
}

void Unit::AttackEnd() {
	RemoveEffect(Effect::ATTACKING);
	behavior_->AttackEnd();
}

void Unit::UnitCollide(AbstractUnit* unit) {
	double x1, x2, y1, y2;
	double cx1, cy1, cx2, cy2;
	double dx1, dy1, dx2, dy2;

	this->GetVector(dx1, dy1);
	unit->GetVector(dx2, dy2);

	this->GetHitbox(x1, y1, x2, y2);
	cx1 = (x1 + x2) / 2.0;
	cy1 = (y1 + y2) / 2.0;

	unit->GetHitbox(x1, y1, x2, y2);
	cx2 = (x1 + x2) / 2.0;
	cy2 = (y1 + y2) / 2.0;

	if (abs(cx1 - cx2) < width_ && abs(cy1 - cy2) < height_) {
		double vx, vy;
		vx = width_ - abs(cx1 - cx2);
		vy = height_ - abs(cy1 - cy2);
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

	if (abs(cx1 - cx2) < width_ * 0.8 && abs(cy1 - cy2) < height_ * 0.8) {
		double vx, vy;
		vx = width_ - abs(cx1 - cx2);
		vy = height_ - abs(cy1 - cy2);
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

void Unit::DoAction() {
	RemoveEffect(Effect::MOVING);
	if (!HasEffect(Effect::UNDER_CONTROL)) {
		behavior_->DoAction();
	}

	double EPS = 0.0001;
	if (abs(dx_) > EPS || abs(dy_) > EPS) {
		AddEffect(Effect::MOVING);
		if (abs(dx_) > EPS) is_right_side = !(dx_ < 0.0);
	}

	int x, y;
	x = floor(x_);
	y = floor(y_);
	int x1, x2, y1, y2;
	x1 = (x + deltaX_) / game_map_->GetBlockSize();
	y1 = (y + deltaY_) / game_map_->GetBlockSize();
	x2 = (x + deltaX_ + width_ - 1) / game_map_->GetBlockSize();
	y2 = (y + deltaY_ + height_ - 1) / game_map_->GetBlockSize();

	std::unordered_set<AbstractUnit*> used_units;
	for (int cur_y = y1; cur_y <= y2; ++cur_y) {
		for (int cur_x = x1; cur_x <= x2; ++cur_x) {
			auto units = game_map_->GetUnitsInBlock(cur_x, cur_y);
			for (AbstractUnit* unit : (*units)) {
				if (unit == this) continue;
				if (used_units.find(unit) != used_units.end()) continue;
				used_units.insert(unit);
				UnitCollide(unit);
			}
		}
	}
}

void Unit::Move() {
	double EPS = 1e-5;
	if (abs(dx_) < EPS && abs(dy_) < EPS) return;
	DeleteUnitFromMap();
	int x, y;
	x = floor(x_);
	y = floor(y_);
	int x1, x2, y1, y2;
	x1 = (x + deltaX_) / game_map_->GetBlockSize() - 1;
	y1 = (y + deltaY_) / game_map_->GetBlockSize() - 1;
	x2 = (x + deltaX_ + width_ - 1) / game_map_->GetBlockSize() + 1;
	y2 = (y + deltaY_ + height_ - 1) / game_map_->GetBlockSize() + 1;

	double up, down, left, right;
	left = x_ + deltaX_;
	up = y_ + deltaY_;
	right = left + width_;
	down = up + height_;

	for (int cur_y = y1; cur_y <= y2; ++cur_y) {
		for (int cur_x = x1; cur_x <= x2; ++cur_x) {
			if (!game_map_->IsBlockInMap(cur_x, cur_y))
				continue;
			if (CanMoveOnBlock(cur_x, cur_y))
				continue;

			SDL_Rect block = game_map_->GetBlockRect(cur_x, cur_y);
			double inner_up, inner_down, inner_left, inner_right;
			double area;
			inner_left = std::max(left + dx_, (double)block.x);
			inner_up = std::max(up + dy_, (double)block.y);
			inner_right = std::min(right + dx_, (double)block.x + block.w);
			inner_down = std::min(down + dy_, (double)block.y + block.h);
			if ((inner_right - inner_left) < EPS) continue;
			if ((inner_down - inner_up) < EPS) continue;
			area = (inner_right - inner_left) * (inner_down - inner_up);
			if (area < EPS) continue;
			int ind = -1; // collision from
			// 0 - up
			// 1 - left
			// 2 - down
			// 3 - right
			double min_coef = 0.0;
			double cur_coef, up_dist, left_dist, down_dist, right_dist;

			up_dist = (down + dy_) - block.y;
			if ((up + dy_) < block.y && (down + dy_) >= block.y &&
				(right + dx_) >= block.x && (left + dx_) <= (block.x + block.w) &&
				up_dist > EPS && dy_ > EPS) {
				cur_coef = up_dist / dy_;
				if (ind == -1 || (ind >= 0 && cur_coef < min_coef)) {
					ind = 0;
					min_coef = cur_coef;
				}
			}

			left_dist = (right + dx_) - block.x;
			if ((left + dx_) < block.x && (right + dx_) >= block.x &&
				(down + dy_) >= block.y && (up + dy_) < (block.y + block.h) &&
				left_dist > EPS && dx_ > EPS) {
				cur_coef = left_dist / dx_;
				if (ind == -1 || (ind >= 0 && cur_coef < min_coef)) {
					ind = 1;
					min_coef = cur_coef;
				}
			}

			down_dist = (up + dy_) - (block.y + block.h);
			if ((up + dy_) < (block.y + block.h) && (down + dy_) > (block.y + block.h) &&
				(right + dx_) >= block.x && (left + dx_) <= (block.x + block.w) &&
				down_dist < -EPS && dy_ < -EPS) {
				cur_coef = down_dist / dy_;
				if (ind == -1 || (ind >= 0 && cur_coef < min_coef)) {
					ind = 2;
					min_coef = cur_coef;
				}
			}

			right_dist = (left + dx_) - (block.x + block.w);
			if ((left + dx_) < (block.x + block.w) && (right + dx_) > (block.x + block.w) &&
				(down + dy_) >= block.y && (up + dy_) < (block.y + block.h) &&
				right_dist < -EPS && dx_ < -EPS) {
				cur_coef = right_dist / dx_;
				if (ind == -1 || (ind >= 0 && cur_coef < min_coef)) {
					ind = 3;
					min_coef = cur_coef;
				}
			}

			switch (ind) {
			case 0: {
				double need_dy = block.y - down;
				double coef = 1.0 - (need_dy / dy_);
				if (coef > 1.0) coef = 1.0;
				if (coef < 0.0) coef = 0.0;
				dx_ *= coef;
				up += need_dy;
				down += need_dy;
				y_ += need_dy;
				dy_ = 0.0;
				break;
			}
			case 1: {
				double need_dx = block.x - right;
				double coef = 1.0 - (need_dx / dx_);
				if (coef > 1.0) coef = 1.0;
				if (coef < 0.0) coef = 0.0;
				dy_ *= coef;
				left += need_dx;
				right += need_dx;
				x_ += need_dx;
				dx_ = 0.0;
				break;
			}
			case 2: {
				double need_dy = (block.y + block.h) - up;
				double coef = 1.0 - (need_dy / dy_);
				if (coef > 1.0) coef = 1.0;
				if (coef < 0.0) coef = 0.0;
				dx_ *= coef;
				up += need_dy;
				down += need_dy;
				y_ += need_dy;
				dy_ = 0.0;
				break;
			}
			case 3: {
				double need_dx = (block.x + block.w) - left;
				double coef = 1.0 - (need_dx / dx_);
				if (coef > 1.0) coef = 1.0;
				if (coef < 0.0) coef = 0.0;
				dy_ *= coef;
				left += need_dx;
				right += need_dx;
				x_ += need_dx;
				dx_ = 0.0;
				break;
			}
			}
		}
	}

	x_ += dx_;
	y_ += dy_;
	if (x_ < 0) {
		x_ = 0;
	}
	if (y_ < 0) {
		y_ = 0;
	}
	if (x_ > (8 * game_map_->GetWidth() - width_)) {
		x_ = (8 * game_map_->GetWidth() - width_);
	}
	if (y_ > (8 * game_map_->GetHeight() - height_)) {
		y_ = (8 * game_map_->GetHeight() - height_);
	}
	dx_ = 0.0;
	dy_ = 0.0;
	InsertUnitToMap();
}

void Unit::Draw(SDL_Renderer* renderer, Camera* camera) {
	SDL_Rect from, to;
	// stay [0, 0]
	// move [1, texture_move_num_]
	// attack [texture_move_num_ + 1, texture_move_num_ + texture_attack_num_]
	texture_cur_delay_++;
	texture_cur_delay_ %= texture_delay_;
	if (texture_cur_delay_ == 0) {
		if (!HasEffect(Effect::MOVING) && !HasEffect(Effect::ATTACKING)) {
			texture_pos_ = 0;
		}
		if (HasEffect(Effect::MOVING)) {
			texture_pos_++;
			if (texture_pos_ < 1 || texture_pos_ > texture_move_num_) {
				texture_pos_ = 1;
			}
		}
		if (HasEffect(Effect::ATTACKING)) {
			texture_pos_++;
			if (texture_pos_ < (texture_move_num_ + 1) || texture_pos_ >(texture_move_num_ + texture_attack_num_)) {
				texture_pos_ = texture_move_num_ + 1;
			}
			if (texture_pos_ == (texture_move_num_ + texture_attack_num_)) {
				AttackEnd();
			}
		}
	}
	from.x = 16 * texture_pos_;
	from.y = (is_right_side ? 0 : 16);
	from.w = 16;
	from.h = 16;
	to.x = -camera->GetCornerX(renderer) + x_;
	to.y = -camera->GetCornerY(renderer) + y_;
	to.w = 16;
	to.h = 16;
	SDL_RenderCopy(renderer, texture_manager_->GetTexture(texture_ind_), &from, &to);



	SDL_Rect hitbox;
	hitbox.x = to.x + deltaX_;
	hitbox.y = to.y + deltaY_;
	hitbox.w = width_;
	hitbox.h = height_;
    SDL_RenderDrawRect(renderer, &hitbox);

	if (type_ == UnitType::Ground) {
		int life_len = (hitbox.w * health_) / max_health_;
		SDL_RenderDrawLine(renderer, hitbox.x, hitbox.y - 2, hitbox.x + hitbox.w - 1, hitbox.y - 2);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawLine(renderer, hitbox.x, hitbox.y - 2, hitbox.x + life_len - 1, hitbox.y - 2);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	}
}

// to do : change this shit
AbstractUnit* Unit::FindEnemyInRadius(int radius) {
	int cx, cy; // unit center
	cx = x_ + deltaX_ + width_ / 2;
	cy = y_ + deltaY_ + height_ / 2;
	int block_size = game_map_->GetBlockSize();
	for (int y = cy - radius; y <= (cy + radius); y += block_size) {
		int dx = 0; 
		int dy;
		AbstractUnit* unit = NULL;
		do {
			unit = GetEnemyInPoint(cx + dx, y);
			if (unit != NULL) return unit;
			unit = GetEnemyInPoint(cx - dx, y);
			if (unit != NULL) return unit;
			dx += block_size;
			dy = y - cy;
		} while ((dx * dx) + (dy * dy) <= (radius * radius));
	}
	return NULL;
}

AbstractUnit* Unit::GetEnemyInPoint(int x, int y) {
	if (!game_map_->IsPositionInMap(x, y)) 
		return NULL;

	uint32_t x_block = x / game_map_->GetBlockSize();
	uint32_t y_block = y / game_map_->GetBlockSize();
	//game_map_->SetBlock(x_block, y_block, 1);
	std::unordered_set<AbstractUnit*>* units_map = game_map_->GetUnitsInBlock(x_block, y_block);
	for (AbstractUnit* unit : (*units_map)) {
		if (players_info_->CanAttack(player_, unit->GetPlayer())) {
			return unit;
		}
	}
	return NULL;
}

void Unit::AddEffect(Effect effect) { 
	effects_.set(effect); 
}

void Unit::RemoveEffect(Effect effect) { 
	effects_.reset(effect); 
}

bool Unit::HasEffect(Effect effect) const { 
	return effects_.test(effect); 
}

void Unit::SetTexture(size_t texture_ind, size_t move_cnt, size_t attack_cnt, size_t texture_delay,
	                  size_t deltaX, size_t deltaY, size_t width, size_t height) {
	texture_ind_ = texture_ind;
	texture_move_num_ = move_cnt;
    texture_attack_num_ = attack_cnt;
	texture_delay_ = texture_delay;
	deltaX_ = deltaX;
	deltaY_ = deltaY;
	width_ = width;
	height_ = height;
}

void Unit::SetType(UnitType type) {
	type_ = type;
}

void Unit::SetBehavior(Behavior* behavior) { 
	behavior_ = behavior; 
}

void Unit::SetPlayer(size_t player) {
	player_ = player;
}

void Unit::SetPlayersInfo(PlayersInfo* players_info) {
	players_info_ = players_info;
}

size_t Unit::GetPlayer() {
	return player_;
}

void Unit::DeadCheck() {
	if (behavior_ != NULL) {
		behavior_->DeadCheck();
	}
}

bool Unit::IsAlive() {
	return alive_;
}

void Unit::Die() {
	alive_ = false;
}