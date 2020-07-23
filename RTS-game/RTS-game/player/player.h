#pragma once

#include <unordered_set>

#include "../units/abstract_unit.h"
#include "../units/abstract_unit_factory.h"

class Player {
protected:
	size_t num_ = 0;

	std::unordered_set<AbstractUnit*> army_;
public:
	Player();
	Player(size_t num);
	~Player();

	void DoAction();
	void Draw(SDL_Renderer* renderer, Camera* camera) const;

	void AddUnit(AbstractUnit* unit);
	void DeleteUnit(AbstractUnit* unit);
};