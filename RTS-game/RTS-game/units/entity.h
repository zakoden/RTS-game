#pragma once

#include "drawable_entity.h"

#include <string>

#include "../clock.h"

#include "../managers/texture_manager.h"

#include "../map/camera.h"

/* Represents something on the map
Doesn't have movement, hp, can be passed through
*/
class Entity : public DrawableEntity {
public:
	Entity() = default;

	Entity(TextureManager* texture_manager, int x, int y);

	void Draw(SDL_Renderer* renderer, Camera* camera) override;
};