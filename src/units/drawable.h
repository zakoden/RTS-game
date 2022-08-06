#pragma once

#include <string>

#include <SDL.h>

#include "../map/camera.h"

class Drawable {
public:
	virtual void Draw(SDL_Renderer* renderer, Camera* camera) = 0;

	virtual void SetTexture(size_t texture_ind, size_t move_cnt, size_t attack_cnt,
		size_t texture_delay_move, size_t texture_delay_attack,
		size_t deltaX, size_t deltaY, size_t width, size_t height,
		size_t texture_width, size_t texture_height) = 0;


	virtual uint8_t GetCurrentLayerIndex() = 0;
};