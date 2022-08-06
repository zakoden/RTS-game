#pragma once

#include "drawable_entity.h"
#include "actionable.h"
#include "with_hitbox.h"
#include "with_player.h"

#include "../behaviors/behavior.h"
#include "../player/players_info.h"
#include "../behaviors/status_effects.h"
#include "unit_type.h"

/* Represents an object on the map
* Interface for ImmovableUnit
* Unlike Entity, it has collisionbox and hitbox; hp, attack and belongs to some player
*/
class AbstractImmovableUnit : public DrawableEntity, public Actionable, public WithHitbox, public WithPlayer {
public: 
	virtual ~AbstractImmovableUnit() = default;

	virtual void SetType(UnitType type) = 0;  // TODO Move to Drawable

	virtual void AddEffect(Effect effect) = 0;
	virtual void RemoveEffect(Effect effect) = 0;
	virtual bool HasEffect(Effect effect) const = 0;
};