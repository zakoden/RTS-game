#pragma once

#include "abstract_unit_factory.h"

#include <array>
#include <string>
#include <unordered_map>

#include "../map/game_map.h"
#include "../map/camera.h"

#include "../managers/texture_manager.h"

#include "../behaviors/behavior_random.h"
#include "../behaviors/behavior_tower.h"
#include "../behaviors/behavior_stay.h"
#include "../behaviors/behavior_bullet.h"
#include "../behaviors/behavior_hunter.h"
#include "../behaviors/behavior_around_point.h"

#include "abstract_unit_factory.h"
#include "unit.h"

#include "../player/player.h"
#include "../player/players_info.h"

class UnitFactory : AbstractUnitFactory {
private:
	std::vector<Player*> players_;

	GameMap* game_map_ = NULL;
	TextureManager* texture_manager_ = NULL;
	PlayersInfo* players_info_ = NULL;

public:
	void AddPlayer(Player* player);

	void SetMap(GameMap* game_map);
	void SetTextureManager(TextureManager* texture_manager);
	void SetPlayersInfo(PlayersInfo* players_info);

	AbstractUnit* CreateTest(size_t player, int x, int y) override;
    AbstractUnit* CreateTest1(size_t player, int x, int y) override;
	AbstractUnit* CreateTestHunter(size_t player, int x, int y) override;
	AbstractUnit* CreateTestHunter2(size_t player, int x, int y) override;

	AbstractUnit* CreateBulletFire1(size_t player, int x, int y, int x_to, int y_to) override;
};