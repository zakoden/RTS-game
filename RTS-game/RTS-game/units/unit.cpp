#include "unit.h"

#include <iostream>

// ----------protected-------------

void Unit::InsertUnitToMap() {
	int cx, cy;
	cx = width_ / 2;
	cy = height_ / 2;
	for (int cur_y = 0; cur_y <= cy; cur_y += 8) {
		for (int cur_x = 0; cur_x <= cx; cur_x += 8) {
			int x1, x2, y1, y2;
			x1 = (x_ + deltaX_ + cur_x) / 8;
			y1 = (y_ + deltaY_ + cur_y) / 8;
			x2 = (x_ + deltaX_ + width_ - 1 - cur_x) / 8;
			y2 = (y_ + deltaY_ + height_ - 1 - cur_y) / 8;
			game_map_->AddUnit(this, x1, y1);
			game_map_->AddUnit(this, x1, y2);
			game_map_->AddUnit(this, x2, y1);
			game_map_->AddUnit(this, x2, y2);
		}
	}
}

void Unit::DeleteUnitFromMap() {
	int cx, cy;
	cx = width_ / 2;
	cy = height_ / 2;
	for (int cur_y = 0; cur_y <= cy; cur_y += 8) {
		for (int cur_x = 0; cur_x <= cx; cur_x += 8) {
			int x1, x2, y1, y2;
			x1 = (x_ + deltaX_ + cur_x) / 8;
			y1 = (y_ + deltaY_ + cur_y) / 8;
			x2 = (x_ + deltaX_ + width_ - 1 - cur_x) / 8;
			y2 = (y_ + deltaY_ + height_ - 1 - cur_y) / 8;
			game_map_->DeleteUnit(this, x1, y1);
			game_map_->DeleteUnit(this, x1, y2);
			game_map_->DeleteUnit(this, x2, y1);
			game_map_->DeleteUnit(this, x2, y2);
		}
	}
}

// -----------public---------------

Unit::Unit(int attack, int defense, int max_health, uint32_t speed,
	       size_t texture_num,
	       TextureManager* texture_manager, GameMap* game_map) {
	attack_ = attack;
	defense_ = defense;
	health_ = max_health;
	max_health_ = max_health;
	speed_ = speed;
	texture_num_ = texture_num;
	texture_manager_ = texture_manager;
	game_map_ = game_map;

	behavior_ = NULL;
}

int Unit::GetX() {
	return x_;
}

int Unit::GetY() {
	return y_;
}

void Unit::SetPosition(int x, int y) {
	x_ = x;
	y_ = y;
	InsertUnitToMap();
}

void Unit::SetVector(int dx, int dy) {
	dx_ = dx;
	dy_ = dy;
}

void Unit::VectorApply() {
	if (dx_ == 0 && dy_ == 0) return;
	DeleteUnitFromMap();
	float f_dx = dx_;
	float f_dy = dy_;
	float v_len = std::sqrt(f_dx * f_dx + f_dy * f_dy);
	f_dx = (f_dx * speed_) / v_len;
	f_dy = (f_dy * speed_) / v_len;
	x_ += std::round(f_dx);
	y_ += std::round(f_dy);
	if (x_ < 0) {
		x_ = 0;
	}
	if (y_ < 0) {
		y_ = 0;
	}
	if (x_ > (game_map_->GetWidth() - width_)) {
		x_ = (game_map_->GetWidth() - width_);
	}
	if (y_ > (game_map_->GetHeight() - height_)) {
		y_ = (game_map_->GetHeight() - height_);
	}
	InsertUnitToMap();
}


void Unit::DoAction() { 
	behavior_->DoAction(); 
}

void Unit::Draw(SDL_Renderer* renderer, Camera* camera) const {
	SDL_Rect from, to;
	from.x = 0;
	from.y = 0;
	from.w = 16;
	from.h = 16;
	to.x = -camera->GetCornerX(renderer) + x_;
	to.y = -camera->GetCornerY(renderer) + y_;;
	to.w = 16;
	to.h = 16;
	SDL_RenderCopy(renderer, texture_manager_->GetTexture(texture_num_), &from, &to);
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

void Unit::SetBehavior(Behavior* behavior) { 
	behavior_ = behavior; 
}

void Unit::SetPlayer(size_t player) {
	player_ = player;
}