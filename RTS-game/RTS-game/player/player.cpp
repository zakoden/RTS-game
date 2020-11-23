#include "player.h"

Player::Player() {
}

Player::Player(uint8_t num) {
	num_ = num;
}


Player::~Player() {
}

uint8_t Player::GetNum() const { return num_; }

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
			MovableUnit* unit = *it;
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

void Player::UnitsToDraw(std::vector<std::pair<int, MovableUnit*>> &out) {
	for (auto& unit : army_) {
		out.push_back({unit->GetLegsY(), unit});
	}
}

void Player::AddUnit(MovableUnit* unit) {
	army_.insert(unit);
}

void Player::DeleteUnit(MovableUnit* unit) {
	army_.erase(unit);
}