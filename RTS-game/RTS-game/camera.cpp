#include "camera.h"

void Camera::SetScale(float new_scale) {
	scale_ = new_scale;
}

void Camera::AddScale(float add_scale) {
	scale_ += add_scale;
}

void Camera::MulScale(float mul_scale) {
	scale_ *= mul_scale;
}

float Camera::GetScale() {
	return scale_;
}

void Camera::Move(int32_t dx, int32_t dy) {
	x_ += dx;
	y_ += dy;
}

void Camera::MoveTo(int32_t x, int32_t y) {
	x_ = x;
	y_ = y;
}

int32_t Camera::GetX() {
	return x_;
}

int32_t Camera::GetY() {
	return y_;
}

int32_t Camera::GetCornerX(SDL_Renderer* renderer) {
	int32_t w, h;
	SDL_GetRendererOutputSize(renderer, &w, &h);
	return int32_t(x_ - w * 0.5 / scale_);
}

int32_t Camera::GetCornerY(SDL_Renderer* renderer) {
	int32_t w, h;
	SDL_GetRendererOutputSize(renderer, &w, &h);
	return int32_t(y_ - h * 0.5 / scale_);
}
