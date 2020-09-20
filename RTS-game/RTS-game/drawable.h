#pragma once

#include "SDL.h"

#include "map/camera.h"

class Drawable {
	virtual void Draw(SDL_Renderer* renderer, Camera* camera) = 0;
};