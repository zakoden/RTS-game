#pragma once

#include "abstract_unit.h"

class Unit : public AbstractUnit {
protected:
	// Атака, защита, текущее здоровье, максимальное здоровье
	int attack_, defense_, health_, max_health_;
	uint32_t speed_;					 // Скорость
	SDL_Texture* texture_ = NULL;		 // Текстура
	Cell position_ = {-1, -1};			 // Позиция
	std::bitset<EFFECT_SIZE> effects_;   // Битсет из эффектов
	std::string name_;					 // Имя
	std::shared_ptr<Behavior> behavior;  // Поведение

public:
	Unit() = default;

	// Конструктор, берущий атаку, защиту, макс. здоровье, скорость, текстуру, имя
	Unit(int attack, int defense, int max_health, uint32_t speed,
		SDL_Texture* texture, const std::string& name)
	: attack_(attack), defense_(defense), health_(max_health),
		max_health_(max_health), speed_(speed), texture_(texture), name_(name) {}

	virtual void Move(const Cell& destination) override;

	virtual void Attack(AbstractUnit& other) override;

	virtual SDL_Texture* Draw() const override;

	virtual ~Unit() = default;
};