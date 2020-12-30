#pragma once

#include "drawable.h"

#include <string>

#include "../clock.h"

#include "../managers/texture_manager.h"

#include "../map/camera.h"

class DrawableEntity : public Drawable {
protected:

	// --cosmetic--
	bool is_right_side = true;
	size_t texture_ind_;
	size_t texture_move_num_ = 0;
	size_t texture_attack_num_ = 0;
	size_t texture_pos_ = 0;
	size_t texture_cur_delay_ = 0;
	size_t texture_delay_move_ = 1;
	size_t texture_delay_attack_ = 1;
	TextureManager* texture_manager_;
	std::string name_;

	// --geometry--
	double x_ = 110.0, y_ = 110.0;
	// hitbox delta
	int deltaX_ = 0, deltaY_ = 0;
	// hitbox size
	int width_ = 16, height_ = 16;
	// texture size
	int texture_width_ = 24, texture_height_ = 24;
	/*
	texture
	-----------------------
	|     /|\             |
	|      | deltaY       |
	|     \|/             |
	|      --------       |
	|deltaX|hitbox|       |
	|<---->|      |       |
	|      --------       |
	-----------------------
	*/

public:
	virtual void SetTexture(size_t texture_ind, size_t move_cnt, size_t attack_cnt,
		size_t texture_delay_move, size_t texture_delay_attack,
		size_t deltaX, size_t deltaY, size_t width, size_t height,
		size_t texture_width, size_t texture_height) override;

	virtual void SetName(std::string name);
};