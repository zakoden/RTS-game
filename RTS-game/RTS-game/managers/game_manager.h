#pragma once

#include <iostream>
#include <vector>

#include "SDL.h"

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


class GameManager {
protected:
	SDL_Window* window_;
	SDL_Renderer* renderer_;
	TextureManager* texture_manager_;
	Camera* camera_;
	GameMap* game_map_;
	bool close_ = false;
	UnitFactory* unit_factory_;
	std::vector<Player*> players_;
	PlayersInfo* players_info_;

	double camera_h_;
	double ground_h_, air_h_;
	double screen_h_ = 1.0;
	int scale_status_ = 1;
public:
	GameManager();
	~GameManager();

	int Init();
	void Run();
	void RunStep();
};