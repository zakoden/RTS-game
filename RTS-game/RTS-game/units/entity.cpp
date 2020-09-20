#include "entity.h"

Entity::Entity(TextureManager* texture_manager, int x, int y)
	: texture_manager_(texture_manager)
	, x_(x)
	, y_(y)
	{}

void Entity::Draw(SDL_Renderer* renderer, Camera* camera) {
	SDL_Rect from, to;
	from.x = texture_width_ * texture_pos_;
	from.y = (is_right_side ? 0 : texture_height_);
	from.w = texture_width_;
	from.h = texture_height_;
	to.x = -camera->GetCornerX(renderer) + x_;
	to.y = -camera->GetCornerY(renderer) + y_;
	to.w = texture_width_;
	to.h = texture_height_;
	SDL_RenderCopy(renderer, texture_manager_->GetTexture(texture_ind_), &from, &to);
}

void Entity::SetTexture(size_t texture_ind, size_t move_cnt, size_t attack_cnt,
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