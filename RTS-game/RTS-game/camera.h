#pragma once

#include <cstdint>

#include "SDL.h"

class Camera {
private:
	float scale_ = 1.0;
	int32_t x_ = 0, y_ = 0;
public:
	void SetScale(float new_scale);
	void AddScale(float add_scale);
	void MulScale(float mul_scale);
	float GetScale();
	void Move(int32_t dx, int32_t dy);
	void MoveTo(int32_t x, int32_t y);
	int32_t GetX();
	int32_t GetY();
	int32_t GetCornerX(SDL_Renderer* renderer);
	int32_t GetCornerY(SDL_Renderer* renderer);
};