#include "unit.h"

#include <iostream>

SDL_Texture* Unit::Draw() const {
	std::cout << "VVVVVV" << std::endl;
	return NULL;
}

void Unit::Move(uint32_t x, uint32_t y) {
	std::cout << "Gay" << std::endl;
}

void Unit::Attack(AbstractUnit& other) {
	std::cout << "CHARGE!" << std::endl;
}

void Unit::SetPosition(uint32_t x, uint32_t y) { x_ = x; y_ = y; }
void Unit::SetBehavior(Behavior* behavior) { behavior_.reset(behavior); }

std::string Unit::GetName() const { return name_; }

void Unit::AddEffect(Effect effect) { effects_.set(effect); }

void Unit::RemoveEffect(Effect effect) { effects_.reset(effect); }

bool Unit::HasEffect(Effect effect) const { return effects_.test(effect); }