#include "entity.h"

Entity::Entity(TextureManager* texture_manager, int x, int y, uint8_t cur_layer_ind) {
	texture_manager_ = texture_manager;
	x_ = x;
	y_ = y;
	cur_layer_ind_ = cur_layer_ind;
}

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
