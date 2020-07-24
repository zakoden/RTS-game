#include "player.h"

Player::Player() {
}

Player::Player(size_t num) {
	num_ = num;
}


Player::~Player() {
}

void Player::DoAction() {
	// to do
	for (auto& unit : army_) {
		unit->DoAction();
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