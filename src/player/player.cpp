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
	for (auto& building : buildings_) {
		if (building->IsAlive()) {
			building->DeadCheck();
			building->DoAction();
		}
	}


	{
		auto it = army_.begin();
		while (it != army_.end()) {
			if ((*it)->IsAlive()) {
				it++;
			}
			else {
				MovableUnit* unit = *it;
				dead_.insert(unit);
				it = army_.erase(it);
			}
		}
	}

	{
		auto it = buildings_.begin();
		while (it != buildings_.end()) {
			if ((*it)->IsAlive()) {
				it++;
			}
			else {
				Building* building = *it;
				destroyed_.insert(building);
				it = buildings_.erase(it);
			}
		}
	}

	// dead check
	for (auto& unit : army_) {
		if (unit->IsAlive()) {
			unit->DeadCheck();
		}
	}
	for (auto& building : buildings_) {
		if (building->IsAlive()) {
			building->DeadCheck();
		}
	}

	player_owner_->DeadCheck();
}

void Player::Move() {
	for (auto& unit : army_) {
		unit->Move();
	}
}

// Deprecated
void Player::Draw(SDL_Renderer* renderer, Camera* camera) const {
	for (auto& unit : army_) {
		unit->Draw(renderer, camera);
	}
}

void Player::UnitsToDraw(std::vector<std::pair<int, Drawable*>> &out) {
	for (MovableUnit* unit : army_) {
		out.push_back({unit->GetLegsY(), unit});
	}
	for (Building* building : buildings_) {
		out.push_back({ building->GetLegsY(), building });
	}
}

void Player::AddUnit(MovableUnit* unit) {
	army_.insert(unit);
}

void Player::AddBuilding(Building* building) {
	buildings_.insert(building);
}

void Player::DeleteUnit(MovableUnit* unit) {
	army_.erase(unit);
}