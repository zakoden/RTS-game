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
	MapLayer* active_layer_ = NULL;
	Camera* camera_ = NULL;
	Player* player_ = NULL;

	MovableUnit* control_unit_ = NULL;
public:
	UserManager(Camera* camera);
	~UserManager();

	Player* GetPlayer() const;

	void SetActiveLayer(MapLayer* active_layer);
	void SetPlayer(Player* player);
	void Draw(SDL_Renderer* renderer, Camera* camera);
	void DoAction(SDL_Renderer* renderer);
	void DeadCheck() override;
};