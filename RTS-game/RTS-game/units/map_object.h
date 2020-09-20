#pragma once

#include "abstract_entity.h"

/* Represents an object on the map
* Unlike Entity, it has hp, collisionbox, hitbox and belongs to some player
* Unlike Unit, it cannot move or attack
*/
class MapObject : public AbstractEntity {
public:
	virtual ~MapObject() = default;
};