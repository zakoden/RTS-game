#pragma once

#include "abstract_unit.h"

#include <bitset>
#include <memory>

#include "../status_effects.h"
#include "../behaviors/behavior.h"

class Unit : public AbstractUnit {
protected:
	int attack_, defense_, health_, max_health_;
	uint32_t speed_;					 // Movement speed
	SDL_Texture* texture_ = NULL;		 // Texture
	uint32_t x_, y_;   // Coordinates of position
	std::bitset<EFFECT_SIZE> effects_;   // Effects, displayed by bitset
	std::string name_; 
	std::shared_ptr<Behavior> behavior_;

public:
	Unit() = default;

	Unit(int attack, int defense, int max_health, uint32_t speed,
		SDL_Texture* texture, const std::string& name)
	: attack_(attack), defense_(defense), health_(max_health),
		max_health_(max_health), speed_(speed), texture_(texture), name_(name) {}

	// Moves this unit to destination
	virtual void Move(uint32_t x, uint32_t y) override;

	// Makes this unit to attack other
	virtual void Attack(AbstractUnit& other) override;

	// Draws and returns texture of this unit
	virtual SDL_Texture* Draw() const override;

	void SetPosition(uint32_t x, uint32_t y);	 // Assigns new position
	void SetBehavior(Behavior* behavior);        // Assigns new behavior

	std::string GetName() const;

	void AddEffect(Effect effect);		  // Adds effect to this unit
	void RemoveEffect(Effect effect);	  // Removes effect from this unit
	bool HasEffect(Effect effect) const;  // Returns true if this unit has that effect
};