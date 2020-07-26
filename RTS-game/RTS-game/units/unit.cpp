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

Unit::Unit(int attack, int defense, int max_health, double speed,
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

void Unit::SetType(UnitType type) {
	type_ = type;
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


void Unit::VectorApply() {
	double EPS = 1e-5;
	if (abs(dx_) < EPS && abs(dy_) < EPS) return;
	DeleteUnitFromMap();
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
	InsertUnitToMap();
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
	if (type_ == UnitType::Ground) {
		int life_len = (to.w * health_) / max_health_;
		SDL_RenderDrawLine(renderer, to.x, to.y, to.x + to.w - 1, to.y);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawLine(renderer, to.x, to.y, to.x + life_len - 1, to.y);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	}

	SDL_Rect hitbox;
	hitbox.x = to.x + deltaX_;
	hitbox.y = to.y + deltaY_;
	hitbox.w = width_;
	hitbox.h = height_;
	SDL_RenderDrawRect(renderer, &hitbox);
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