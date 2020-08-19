#pragma once

#include <iostream>
#include <vector>

#include "SDL.h"

#include "../clock.h"

#include "../map/game_map.h"
#include "../map/camera.h"

#include "texture_manager.h"

#include "../behaviors/behavior_random.h"
#include "../behaviors/behavior_tower.h"
#include "../behaviors/behavior_stay.h"

#include "../units/unit_factory.h"
#include "../units/unit.h"

#include "../player/player.h"
#include "../player/players_info.h"

#include "user_manager.h"

class GameManager {
protected:
	SDL_Window* window_ = NULL;
	SDL_Renderer* renderer_ = NULL;
	TextureManager* texture_manager_ = NULL;
	Camera* camera_ = NULL;
	GameMap* game_map_ = NULL;
	bool close_ = false;
	UnitFactory* unit_factory_ = NULL;
	std::vector<Player*> players_;
	PlayersInfo* players_info_ = NULL;
	UserManager* user_manager_ = NULL;

	bool is_fullscreen_ = false;

	// 0 means "no fog of war", 1 means "map is visible, units aren't", 2 means "map is not visible"
	enum FogOfWarType {
		VISIBLE,
		UNITS_HIDDEN,
		MAP_HIDDEN
	} fog_of_war_mode_ = MAP_HIDDEN;

	double camera_h_ = 1.0;
	double ground_h_ = 1.0, air_h_ = 1.0;
	double screen_h_ = 1.0;
	int scale_status_ = 1;
public:
	GameManager();
	~GameManager();

	int Init();
	void Run();
	void RunStep();
};