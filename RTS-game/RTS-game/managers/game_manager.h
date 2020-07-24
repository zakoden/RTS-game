#pragma once

#include <iostream>
#include <vector>

#include "SDL.h"

#include "../game_map.h"
#include "../camera.h"

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
public:
	GameManager();
	~GameManager();

	int Init();
	void Run();
	void RunStep();
};