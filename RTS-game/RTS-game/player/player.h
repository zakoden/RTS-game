#pragma once

#include <unordered_set>

#include "../units/movable_unit.h"
#include "../units/abstract_unit_factory.h"

#include "../map/grid.h"
#include "player_owner.h"

class Player {
protected:
	PlayerOwner* player_owner_;
	uint8_t num_ = 0;

	std::unordered_set<MovableUnit*> army_;
	std::unordered_set<Building*> buildings_;

	// For scout behavior, score rewarded is based on
	// Distance from a base, the less distance, the better
	Grid<float> score_from_base_;

public:
	Player();
	Player(uint8_t num);
	~Player();

	uint8_t GetNum() const;

	void SetOwner(PlayerOwner* owner);

	void DoAction();
	void Move();
	void Draw(SDL_Renderer* renderer, Camera* camera) const; 
	void UnitsToDraw(std::vector<std::pair<int, Drawable*>> &out);

	void AddUnit(MovableUnit* unit);
	void AddBuilding(Building* building);
	void DeleteUnit(MovableUnit* unit);
};