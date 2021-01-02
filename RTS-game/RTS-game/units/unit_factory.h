#pragma once

#include "abstract_unit_factory.h"

#include "../map/map_layer.h"
#include "../map/camera.h"

#include "../managers/texture_manager.h"

#include "abstract_unit_factory.h"
#include "unit.h"

#include "../player/player.h"
#include "../player/players_info.h"

class UnitFactory : MovableUnitFactory {
private:
	std::vector<Player*> players_;

	MapLayer* game_map_;
	TextureManager* texture_manager_ = NULL;
	PlayersInfo* players_info_ = NULL;

	void FillUnit(AbstractImmovableUnit* unit, 
		UnitType unit_type, size_t player, Behavior* behavior, int x, int y, std::string name);

public:
	void AddPlayer(Player* player);

	void SetMap(MapLayer* game_map);
	void SetTextureManager(TextureManager* texture_manager);
	void SetPlayersInfo(PlayersInfo* players_info);

	MovableUnit* CreateTest(size_t player, int x, int y) override;
    MovableUnit* CreateTest1(size_t player, int x, int y) override;
	MovableUnit* CreateTestHunter(size_t player, int x, int y) override;
	MovableUnit* CreateTestHunter2(size_t player, int x, int y) override;
	MovableUnit* CreateScout(size_t player, int x, int y, MapPoint center) override;

	MovableUnit* CreateFireSmallPoleax(size_t player, int x, int y) override; 
	MovableUnit* CreateFireSmallSpear(size_t player, int x, int y) override;
	MovableUnit* CreateFireSmallLance(size_t player, int x, int y) override;

	MovableUnit* CreateFireMediumPoleax(size_t player, int x, int y) override;

	MovableUnit* CreateBulletFire1(size_t player, int x, int y, int x_to, int y_to) override;

	MovableUnit* CreateBase(uint8_t player, int x, int y) override;

	Building* CreateSmallHorizontalGrayWall(uint8_t player, int x, int y) override;
	Building* CreateSmallVerticalGrayWall(uint8_t player, int x, int y) override;
	Building* CreateSmallGrayTower(uint8_t player, int x, int y) override;

	Entity* CreateBamboo(int x, int y) override;
};