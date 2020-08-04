#include "user_manager.h"

UserManager::UserManager(GameMap* game_map, Camera* camera) {
	game_map_ = game_map;
	camera_ = camera;
}

UserManager::~UserManager() {
}

void UserManager::AddPlayer(Player* player) {
	player_ = player;
}

void UserManager::Draw(SDL_Renderer* renderer, Camera* camera) {
}

void UserManager::DoAction(SDL_Renderer* renderer) {
	int mouse_x, mouse_y;
	uint32_t mouse_state = SDL_GetMouseState(&mouse_x, &mouse_y);
	mouse_x = camera_->ToMapX(renderer, mouse_x);
	mouse_y = camera_->ToMapY(renderer, mouse_y);
	if (game_map_->IsPositionInMap(mouse_x, mouse_y)) {
		int block_x, block_y;
		block_x = mouse_x / game_map_->GetBlockSize();
		block_y = mouse_y / game_map_->GetBlockSize();
		if (mouse_state & SDL_BUTTON(SDL_BUTTON_LEFT)) {
			if (!game_map_->GetUnitsInBlock(block_x, block_y)->empty()) {
				if (control_unit_ != NULL) {
					control_unit_->RemoveEffect(Effect::UNDER_CONTROL);
					control_unit_ = NULL;
				}
				control_unit_ = *game_map_->GetUnitsInBlock(block_x, block_y)->begin();
				control_unit_->AddEffect(Effect::UNDER_CONTROL);
			}
		}
	}

	if (control_unit_ != NULL) {
		const Uint8* keyboard_state = SDL_GetKeyboardState(NULL);
		double dx = 0.0, dy = 0.0;
		if (keyboard_state[SDL_SCANCODE_W]) {
			dy = -1.0;
		}
		if (keyboard_state[SDL_SCANCODE_S]) {
			dy = 1.0;
		}
		if (keyboard_state[SDL_SCANCODE_D]) {
			dx = 1.0;
		}
		if (keyboard_state[SDL_SCANCODE_A]) {
			dx = -1.0;
		}
		control_unit_->SetVector(dx, dy);
		control_unit_->VectorApply();
	}
}

void UserManager::DeadCheck() {
	if (control_unit_ != NULL && !control_unit_->IsAlive()) {
		control_unit_ = NULL;
	}
}