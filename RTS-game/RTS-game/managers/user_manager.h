#pragma once

#include <iostream>
#include <vector>

#include "SDL.h"

#include "../map/game_map.h"

#include "../player/player.h"
#include "../player/players_info.h"
#include "../player/player_owner.h"

#include "../map/camera.h"

class UserManager : public PlayerOwner {
protected:
	GameMap* game_map_ = NULL;
	Camera* camera_ = NULL;
	Player* player_ = NULL;

	MovableUnit* control_unit_ = NULL;
public:
	UserManager(GameMap* game_map, Camera* camera);
	~UserManager();

	Player* GetPlayer() const;

	void SetPlayer(Player* player);
	void Draw(SDL_Renderer* renderer, Camera* camera);
	void DoAction(SDL_Renderer* renderer);
	void DeadCheck() override;
};