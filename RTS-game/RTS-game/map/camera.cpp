#include "camera.h"

void Camera::SetScale(float new_scale) {
	scale_ = new_scale;
	scale_ = ConvertScale(scale_);
}

void Camera::AddScale(float add_scale) {
	scale_ += add_scale;
	scale_ = ConvertScale(scale_);
}

void Camera::MulScale(float mul_scale) {
	scale_ *= mul_scale;
	scale_ = ConvertScale(scale_);
}

double Camera::GetScale() {
	return scale_;
}

double Camera::ConvertScale(double scale) {
	/*
	if (scale < 0.2) {
		scale = 0.2;
		return scale;
	}
	if (scale > 20.0) {
		scale = 20.0;
		return scale;
	}
	int l, r, mid;
	double cur;
	if (scale > 1.0) {
		l = 1;
		r = 20;
		while (l < r) {
			mid = (l + r + 1) / 2;
			cur = (double)mid;
			if (cur <= scale) {
				l = mid;
			}
			else {
				r = mid - 1;
			}
		}
		scale = (double)l;
	}
	else {
		l = 1;
		r = 5;
		while (l < r) {
			mid = (l + r + 1) / 2;
			cur = (double)mid * 0.2;
			if (cur <= scale) {
				l = mid;
			}
			else {
				r = mid - 1;
			}
		}
		scale = (double)l * 0.2;
	}
	*/
	return scale;
}

void Camera::Move(double dx, double dy) {
	x_ += dx;
	y_ += dy;
}

void Camera::MoveTo(double x, double y) {
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
