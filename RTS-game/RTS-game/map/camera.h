#pragma once

#include <cstdint>

#include "SDL.h"

class Camera {
private:
	double scale_ = 1.0;
	double x_ = 0.0, y_ = 0.0;
public:
	void SetScale(float new_scale);
	void AddScale(float add_scale);
	void MulScale(float mul_scale);
	double GetScale();
	double ConvertScale(double scale);
	double ToMapX(SDL_Renderer* renderer, int x);
	double ToMapY(SDL_Renderer* renderer, int y);
	void Move(double dx, double dy);
	void MoveTo(double x, double y);
	int32_t GetX();
	int32_t GetY();
	int32_t GetCornerX(SDL_Renderer* renderer);
	int32_t GetCornerY(SDL_Renderer* renderer);
};