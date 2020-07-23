#pragma once

#include "SDL.h"

class AbstractUnit {
public:
	// Moves this unit to destination
	virtual void Move(uint32_t x, uint32_t y) = 0;

	// Makes this unit to attack other
	virtual void Attack(AbstractUnit& other) = 0;
	
	// Draws and returns texture of this unit
	virtual SDL_Texture* Draw() const = 0;
};