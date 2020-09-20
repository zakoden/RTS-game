#include "units/unit.h"

void Unit::Draw(SDL_Renderer* renderer, Camera* camera) {
	SDL_Rect from, to;
	// stay [0, 0]
	// move [1, texture_move_num_]
	// attack [texture_move_num_ + 1, texture_move_num_ + texture_attack_num_]
	++texture_cur_delay_;
	if (HasEffect(Effect::MOVING)) {
		texture_cur_delay_ %= texture_delay_move_;
	}
	else {
		texture_cur_delay_ %= texture_delay_attack_;
	}
	if (texture_cur_delay_ == 0) {
		if (!HasEffect(Effect::MOVING) && !HasEffect(Effect::ATTACKING)) {
			texture_pos_ = 0;
		}
		if (HasEffect(Effect::MOVING)) {
			texture_pos_++;
			if (texture_pos_ < 1 || texture_pos_ > texture_move_num_) {
				texture_pos_ = 1;
			}
		}
		if (HasEffect(Effect::ATTACKING)) {
			texture_pos_++;
			if (texture_pos_ < (texture_move_num_ + 1) || texture_pos_ >(texture_move_num_ + texture_attack_num_)) {
				texture_pos_ = texture_move_num_ + 1;
			}
			if (texture_pos_ == (texture_move_num_ + texture_attack_num_)) {
				AttackEnd();
			}
		}
	}
	from.x = texture_width_ * texture_pos_;
	from.y = (is_right_side ? 0 : texture_height_);
	from.w = texture_width_;
	from.h = texture_height_;
	to.x = -camera->GetCornerX(renderer) + x_;
	to.y = -camera->GetCornerY(renderer) + y_;
	to.w = texture_width_;
	to.h = texture_height_;
	SDL_RenderCopy(renderer, texture_manager_->GetTexture(texture_ind_), &from, &to);



	SDL_Rect hitbox;
	hitbox.x = to.x + deltaX_;
	hitbox.y = to.y + deltaY_;
	hitbox.w = width_;
	hitbox.h = height_;
	//SDL_RenderDrawRect(renderer, &hitbox);

	/*
	if (type_ == UnitType::Ground) {
		int life_len = (hitbox.w * health_) / max_health_;
		SDL_RenderDrawLine(renderer, hitbox.x, hitbox.y - 2, hitbox.x + hitbox.w - 1, hitbox.y - 2);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderDrawLine(renderer, hitbox.x, hitbox.y - 2, hitbox.x + life_len - 1, hitbox.y - 2);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
	}
	*/

}



#include "map/game_map.h"

void GameMap::BlockDraw(SDL_Renderer* renderer, Camera* camera, uint32_t x, uint32_t y, uint8_t block) {
	SDL_Texture* tiles_to_use = (block == UINT8_MAX - 1) ? fogged_tiles_ : tiles_;
	if (block == UINT8_MAX || block == UINT8_MAX - 1)
		block = blocks_[GetInd(x, y)];
	if (!units_in_block_[GetInd(x, y)].empty()) {
		//	block = 0;
	}

	if (rectangle_blocks_[block].x == -1) {
		rectangle_blocks_[block].x = (block % BLOCKS_IN_LINE) * BLOCK_SIZE;
		rectangle_blocks_[block].y = (block / BLOCKS_IN_LINE) * BLOCK_SIZE;
		rectangle_blocks_[block].w = BLOCK_SIZE;
		rectangle_blocks_[block].h = BLOCK_SIZE;
	}

	SDL_Rect to;
	to.x = -camera->GetCornerX(renderer) + x * BLOCK_SIZE;
	to.y = -camera->GetCornerY(renderer) + y * BLOCK_SIZE;
	to.w = BLOCK_SIZE;
	to.h = BLOCK_SIZE;
	SDL_RenderCopy(renderer, tiles_to_use, &rectangle_blocks_[block], &to);
}

void GameMap::Draw(SDL_Renderer* renderer, Camera* camera, uint8_t player_num) {
	//SDL_RenderCopy(renderer, tiles_, NULL, NULL);
	for (uint32_t y = 0; y < GetHeight(); ++y) {
		for (uint32_t x = 0; x < GetWidth(); ++x) {
			if (player_num == UINT8_MAX || IsCellUncovered({ x, y }, player_num)) {
				BlockDraw(renderer, camera, x, y);
			}
		}
	}
}

void GameMap::ApplyMask(SDL_Renderer* renderer, Camera* camera, int player_num, uint8_t block_num) {
	for (uint32_t y = 0; y < GetHeight(); ++y) {
		for (uint32_t x = 0; x < GetWidth(); ++x) {
			if (!IsCellUncovered({ x, y }, player_num)) {
				BlockDraw(renderer, camera, x, y, block_num);
			}
		}
	}
}
