#include "unit.h"

#include <algorithm>
#include <iostream>

// -----------public---------------

Unit::Unit(int attack, int defense, int max_health, double speed,
	TextureManager* texture_manager, MapLayer* game_map) {
	this->attack_ = attack;
	this->defense_ = defense;
	this->health_ = max_health;
	this->max_health_ = max_health;
	this->game_map_ = game_map;
	this->texture_manager_ = texture_manager;
	this->speed_ = speed;
}

int Unit::GetSpeed() { return speed_; }

void Unit::GetVector(double& dx, double& dy) {
	dx = dx_;
	dy = dy_;
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
	x_ = std::clamp(x_ + dx_, 0.0, 8.0 * game_map_->GetWidth() - width_);
	y_ = std::clamp(y_ + dy_, 0.0, 8.0 * game_map_->GetHeight() - height_);
}

void Unit::SetCommandPoint(int x, int y) {
	AddEffect(Effect::HAS_COMMAND_POINT);
	command_x_ = x;
	command_y_ = y;
}

void Unit::GetCommandPoint(int& x, int& y) {
	x = command_x_;
	y = command_y_;
}


void Unit::DoAction() {
	RemoveEffect(Effect::MOVING);
	if (!HasEffect(Effect::UNDER_CONTROL)) {
		behavior_->DoAction();
	}

	const double EPS = 0.0001;
	if (abs(dx_) > EPS || abs(dy_) > EPS) {
		AddEffect(Effect::MOVING);
		if (abs(dx_) > EPS) is_right_side = !(dx_ < 0.0);
	}

	ImmovableUnit::DoAction();
}

void Unit::Move() {
	if (update_fog_of_war_.Ding())
		UncoverNearbyCells();
	update_fog_of_war_.Tick();

	const double EPS = 1e-5;
	if (abs(dx_) < EPS && abs(dy_) < EPS) return;
	DeleteUnitFromMap();
	int x = static_cast<int>(x_), y = static_cast<int>(y_);
	int x1 = (x + deltaX_) / game_map_->GetBlockSize() - 1;
	int y1 = (y + deltaY_) / game_map_->GetBlockSize() - 1;
	int x2 = (x + deltaX_ + width_ - 1) / game_map_->GetBlockSize() + 1;
	int y2 = (y + deltaY_ + height_ - 1) / game_map_->GetBlockSize() + 1;

	double left = x_ + deltaX_;
	double up = y_ + deltaY_;
	double right = left + width_;
	double down = up + height_;

	for (int cur_y = y1; cur_y <= y2; ++cur_y) {
		for (int cur_x = x1; cur_x <= x2; ++cur_x) {
			if (!game_map_->IsBlockInMap(cur_x, cur_y))
				continue;
			if (CanMoveOnBlock(cur_x, cur_y))
				continue;

			SDL_Rect block = game_map_->GetBlockRect(cur_x, cur_y);
			double inner_left = std::max(left + dx_, (double)block.x);
			double inner_up = std::max(up + dy_, (double)block.y);
			double inner_right = std::min(right + dx_, (double)block.x + block.w);
			double inner_down = std::min(down + dy_, (double)block.y + block.h);
			if ((inner_right - inner_left) < EPS) continue;
			if ((inner_down - inner_up) < EPS) continue;
			double area = (inner_right - inner_left) * (inner_down - inner_up);
			if (area < EPS) continue;
			int ind = -1; // collision from
			// 0 - up
			// 1 - left
			// 2 - down
			// 3 - right
			double min_coef = 0.0, cur_coef;

			double up_dist = (down + dy_) - block.y;
			if ((up + dy_) < block.y && (down + dy_) >= block.y &&
				(right + dx_) >= block.x && (left + dx_) <= (block.x + block.w) &&
				up_dist > EPS && dy_ > EPS) {
				cur_coef = up_dist / dy_;
				if (ind == -1 || (ind >= 0 && cur_coef < min_coef)) {
					ind = 0;
					min_coef = cur_coef;
				}
			}

			double left_dist = (right + dx_) - block.x;
			if ((left + dx_) < block.x && (right + dx_) >= block.x &&
				(down + dy_) >= block.y && (up + dy_) < (block.y + block.h) &&
				left_dist > EPS && dx_ > EPS) {
				cur_coef = left_dist / dx_;
				if (ind == -1 || (ind >= 0 && cur_coef < min_coef)) {
					ind = 1;
					min_coef = cur_coef;
				}
			}

			double down_dist = (up + dy_) - (block.y + block.h);
			if ((up + dy_) < (block.y + block.h) && (down + dy_) > (block.y + block.h) &&
				(right + dx_) >= block.x && (left + dx_) <= (block.x + block.w) &&
				down_dist < -EPS && dy_ < -EPS) {
				cur_coef = down_dist / dy_;
				if (ind == -1 || (ind >= 0 && cur_coef < min_coef)) {
					ind = 2;
					min_coef = cur_coef;
				}
			}

			double right_dist = (left + dx_) - (block.x + block.w);
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
				double coef = std::clamp(1.0 - (need_dy / dy_), 0.0, 1.0);
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
				double coef = std::clamp(1.0 - (need_dx / dx_), 0.0, 1.0);
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
				double coef = std::clamp(1.0 - (need_dy / dy_), 0.0, 1.0);
				dx_ *= coef;
				up += need_dy;
				down += need_dy;
				y_ += need_dy;
				dy_ = 0.0;
				break;
			}
			case 3: {
				double need_dx = (block.x + block.w) - left;
				double coef = std::clamp(1.0 - (need_dx / dx_), 0.0, 1.0);
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

	x_ = std::clamp(x_ + dx_, 0.0, 8.0 * game_map_->GetWidth() - width_);
	y_ = std::clamp(y_ + dy_, 0.0, 8.0 * game_map_->GetHeight() - height_);
	dx_ = 0.0;
	dy_ = 0.0;
	InsertUnitToMap();
}

void Unit::Draw(SDL_Renderer* renderer, Camera* camera) {
	SDL_Rect from, to;
	// stay [0, 0]
	// move [1, texture_move_num_]
	// attack [texture_move_num_ + 1, texture_move_num_ + texture_attack_num_]
	++texture_cur_delay_;
	if (HasEffect(Effect::MOVING)) {
		texture_cur_delay_ %= texture_delay_move_;
	} else {
		texture_cur_delay_ %= texture_delay_attack_;
	}
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
			if (texture_pos_ < (texture_move_num_ + 1) || texture_pos_ > (texture_move_num_ + texture_attack_num_)) {
				texture_pos_ = texture_move_num_ + 1;
			}
			if (texture_pos_ == (texture_move_num_ + texture_attack_num_)) {
				AttackEnd();
			}
		}
	}
	from.x = texture_width_ * texture_pos_;
	from.y = (is_right_side ? 0 : texture_height_);
	from.w = texture_width_;
	from.h = texture_height_;
	to.x = -camera->GetCornerX(renderer) + x_;
	to.y = -camera->GetCornerY(renderer) + y_;
	to.w = texture_width_;
	to.h = texture_height_;
	SDL_RenderCopy(renderer, texture_manager_->GetTexture(texture_ind_), &from, &to);


	
	SDL_Rect hitbox;
	hitbox.x = to.x + deltaX_;
	hitbox.y = to.y + deltaY_;
	hitbox.w = width_;
	hitbox.h = height_;
    //SDL_RenderDrawRect(renderer, &hitbox);

	
	if (type_ == UnitType::Ground) {
		int life_len = (hitbox.w * health_) / max_health_;
		SDL_RenderDrawLine(renderer, hitbox.x, hitbox.y - 2, hitbox.x + hitbox.w - 1, hitbox.y - 2);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawLine(renderer, hitbox.x, hitbox.y - 2, hitbox.x + life_len - 1, hitbox.y - 2);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	}
	
}
