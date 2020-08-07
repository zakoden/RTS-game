#include "player.h"

Player::Player() {
}

Player::Player(size_t num) {
	num_ = num;
}


Player::~Player() {
}

void Player::SetOwner(PlayerOwner* owner) {
	player_owner_ = owner;
}

void Player::DoAction() {
	for (auto& unit : army_) {
		if (unit->IsAlive()) {
			unit->DeadCheck();
			unit->DoAction();
		}
	}

	// dead check
	for (auto& unit : army_) {
		if (unit->IsAlive()) {
			unit->DeadCheck();
		}
	}
	player_owner_->DeadCheck();
	auto it = army_.begin();
	while (it != army_.end()) {
		if ((*it)->IsAlive()) {
			it++;
		} else {
			AbstractUnit* unit = *it;
			it = army_.erase(it);
			delete unit;
		}
	}
}

void Player::Move() {
	for (auto& unit : army_) {
		unit->Move();
	}
}

void Player::Draw(SDL_Renderer* renderer, Camera* camera) const {
	for (auto& unit : army_) {
		unit->Draw(renderer, camera);
	}
}

void Player::AddUnit(AbstractUnit* unit) {
	army_.insert(unit);
}

void Player::DeleteUnit(AbstractUnit* unit) {
	army_.erase(unit);
}