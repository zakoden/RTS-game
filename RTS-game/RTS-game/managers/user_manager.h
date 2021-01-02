#pragma once

#include <iostream>
#include <vector>

#include "SDL.h"

#include "../map/map_layer.h"

#include "../player/player.h"
#include "../player/players_info.h"
#include "../player/player_owner.h"

#include "../map/camera.h"

class UserManager : public PlayerOwner {
protected:
	MapLayer* game_map_ = NULL;
	Camera* camera_ = NULL;
	Player* player_ = NULL;

	MovableUnit* control_unit_ = NULL;
public:
	UserManager(MapLayer* game_map, Camera* camera);
	~UserManager();

	Player* GetPlayer() const;

	void SetPlayer(Player* player);
	void Draw(SDL_Renderer* renderer, Camera* camera);
	void DoAction(SDL_Renderer* renderer);
	void DeadCheck() override;
};