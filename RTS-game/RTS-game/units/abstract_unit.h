#pragma once

#include "SDL.h"

#include "../cell.h"

class AbstractUnit {
public:
	// Двигает юнита на другую клетку
	virtual void Move(const Cell& destination) = 0;

	// Заставляет юнита атаковать другого
	virtual void Attack(AbstractUnit& other) = 0;
	
	virtual SDL_Texture* Draw() const = 0;  // Рисует и возвращает текстуру юнита
};