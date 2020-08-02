#pragma once

#include "../map/game_map.h"

#include "../player/player.h"
#include "../player/players_info.h"


class UserManager {
protected:
	GameMap* game_map_ = NULL;
	Player* player_ = NULL;
public:
	UserManager();
	~UserManager();

	void Draw(SDL_Renderer* renderer, Camera* camera);
};