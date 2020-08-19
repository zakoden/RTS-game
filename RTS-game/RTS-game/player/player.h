#pragma once

#include <unordered_set>

#include "../units/abstract_unit.h"
#include "../units/abstract_unit_factory.h"

#include "player_owner.h"

class Player {
protected:
	PlayerOwner* player_owner_;
	size_t num_ = 0;

	std::unordered_set<AbstractUnit*> army_;
public:
	Player();
	Player(size_t num);
	~Player();

	size_t GetNum() const;

	void SetOwner(PlayerOwner* owner);

	void DoAction();
	void Move();
	void Draw(SDL_Renderer* renderer, Camera* camera) const;

	void AddUnit(AbstractUnit* unit);
	void DeleteUnit(AbstractUnit* unit);
};