#pragma once

#include <bitset>
#include <memory>

#include "SDL.h"

#include "../behaviors/behavior.h"
#include "../cell.h"
#include "../status_effects.h"

class AbstractUnit {
protected:
	// Атака, защита, текущее здоровье, максимальное здоровье
	int attack_ = 0, defense_ = 0, health_ = 0, max_health_ = 0;
	uint32_t speed_ = 0;				  // Скорость
	SDL_Texture* texture_ = NULL;		  // Текстура
	Cell position_ = {-1, -1};		  	  // Позиция
	std::bitset<EFFECT_SIZE> effects_;    // Битсет из эффектов
	std::string name_;					  // Имя
	std::shared_ptr<Behavior> behavior_;  // Поведение

public:
	// Двигает юнита на другую клетку
	virtual void Move(const Cell& destination) = 0;

	// Заставляет юнита атаковать другого
	virtual void Attack(AbstractUnit& other) = 0;
	
	virtual SDL_Texture* Draw() const = 0;  // Рисует и возвращает текстуру юнита

	void SetPosition(const Cell& position);	 // Присваивает новую позицию
	void SetBehavior(Behavior* behavior);    // Присваивает поведение

	std::string GetName() const;  // Возвращает имя юнита

	void AddEffect(Effect effect);		  // Добавляет эффект к юниту
	void RemoveEffect(Effect effect);	  // Удаляет эффект у юнита (если он есть)
	bool HasEffect(Effect effect) const;  // Возвращает 1 если такой эффект есть и 0 иначе

	virtual ~AbstractUnit() = default;
};