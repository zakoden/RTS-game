#include "unit.h"

#include <iostream>

SDL_Texture* Unit::Draw() const {
	std::cout << "VVVVVV" << std::endl;
	return NULL;
}

void Unit::Move(const Cell& destination) {
	std::cout << "Gay" << std::endl;
}

void Unit::Attack(AbstractUnit& other) {
	std::cout << "CHARGE!" << std::endl;
}
