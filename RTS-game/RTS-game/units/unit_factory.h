#pragma once

#include "abstract_unit_factory.h"

#include <array>
#include <string>
#include <unordered_map>

#include "../map/game_map.h"
#include "../map/camera.h"

#include "../managers/texture_manager.h"

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

	void FillUnit(Unit* unit, UnitType unit_type, size_t player, Behavior* behavior, int x, int y);

public:
	void AddPlayer(Player* player);

	void SetMap(GameMap* game_map);
	void SetTextureManager(TextureManager* texture_manager);
	void SetPlayersInfo(PlayersInfo* players_info);

	AbstractUnit* CreateTest(size_t player, int x, int y) override;
    AbstractUnit* CreateTest1(size_t player, int x, int y) override;
	AbstractUnit* CreateTestHunter(size_t player, int x, int y) override;
	AbstractUnit* CreateTestHunter2(size_t player, int x, int y) override;
	AbstractUnit* CreateScout(size_t player, int x, int y, MapPoint center) override;

	AbstractUnit* CreateFireSmallPoleax(size_t player, int x, int y) override; 
	AbstractUnit* CreateFireSmallSpear(size_t player, int x, int y) override;
	AbstractUnit* CreateFireSmallLance(size_t player, int x, int y) override;

	AbstractUnit* CreateFireMediumPoleax(size_t player, int x, int y) override;

	AbstractUnit* CreateBulletFire1(size_t player, int x, int y, int x_to, int y_to) override;

	AbstractUnit* CreateBase(uint8_t player, int x, int y) override;

	AbstractUnit* CreateSmallHorizontalGrayWall(uint8_t player, int x, int y) override;
	AbstractUnit* CreateSmallVerticalGrayWall(uint8_t player, int x, int y) override;
	AbstractUnit* CreateSmallGrayTower(uint8_t player, int x, int y) override;

	AbstractUnit* CreateBamboo(uint8_t player, int x, int y) override;
};