#include "drawable_entity.h"

void DrawableEntity::SetTexture(size_t texture_ind, size_t move_cnt, size_t attack_cnt,
	size_t texture_delay_move, size_t texture_delay_attack,
	size_t deltaX, size_t deltaY, size_t width, size_t height,
	size_t texture_width, size_t texture_height) {
	texture_ind_ = texture_ind;
	texture_move_num_ = move_cnt;
	texture_attack_num_ = attack_cnt;
	texture_delay_move_ = texture_delay_move;
	texture_delay_attack_ = texture_delay_attack;
	deltaX_ = deltaX;
	deltaY_ = deltaY;
	width_ = width;
	height_ = height;
	texture_width_ = texture_width;
	texture_height_ = texture_height;
}

void DrawableEntity::SetName(std::string name) { name_ = name; }

uint8_t DrawableEntity::GetCurrentLayerIndex() {
	return cur_layer_ind_;
}
