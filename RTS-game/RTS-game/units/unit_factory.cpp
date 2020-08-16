#include "unit_factory.h"

void UnitFactory::AddPlayer(Player* player) {
	players_.push_back(player);
}

void UnitFactory::SetMap(GameMap* game_map) {
	game_map_ = game_map;
}

void UnitFactory::SetTextureManager(TextureManager* texture_manager) {
	texture_manager_ = texture_manager;
}

void UnitFactory::SetPlayersInfo(PlayersInfo* players_info) {
	players_info_ = players_info;
}

AbstractUnit* UnitFactory::CreateTest(size_t player, int x, int y) {
	Unit* unit = new Unit(10, 1, 35, 0.5, texture_manager_, game_map_);
	unit->SetTexture(2, 2, 0, 10, 10, 3, 2, 10, 14, 16, 16);
	unit->SetPosition(x, y);
	unit->SetPlayer(player);
	unit->SetPlayersInfo(players_info_);
	BehaviorRandom* behavior = new BehaviorRandom(unit);
	//BehaviorStay* behavior = new BehaviorStay(unit);
	unit->SetBehavior(behavior);
	players_[player]->AddUnit(unit);
	return unit;
}

AbstractUnit* UnitFactory::CreateTest1(size_t player, int x, int y) {
	Unit* unit = new Unit(10, 1, 35, 1.0, texture_manager_, game_map_);
	unit->SetTexture(1, 0, 0, 1, 1, 0, 0, 16, 16, 16, 16);
	unit->SetType(UnitType::Ground);
	unit->SetPlayer(player);
	unit->SetPlayersInfo(players_info_);
	BehaviorTower* behavior = new BehaviorTower(unit, this);
	unit->SetBehavior(behavior);

    unit->SetPosition(x, y);
	players_[player]->AddUnit(unit);
	return unit;
}

AbstractUnit* UnitFactory::CreateTestHunter(size_t player, int x, int y) {
	Unit* unit = new Unit(10 + rand() % 5, 1, 40, 1.0, texture_manager_, game_map_);
	unit->SetTexture(4, 2, 2, 10, 10, 3, 2, 10, 14, 16, 16);
	unit->SetType(UnitType::Ground);
	unit->SetPlayer(player);
	unit->SetPlayersInfo(players_info_);
	BehaviorHunter* inner_behavior = new BehaviorHunter(unit, this);
	//BehaviorAroundPoint* behavior = new BehaviorAroundPoint(unit, this, inner_behavior, x, y, 80);
	unit->SetBehavior(inner_behavior);

	unit->SetPosition(x, y);
	unit->SetCommandPoint(x + 800, y);
	players_[player]->AddUnit(unit);
	return unit;
}

AbstractUnit* UnitFactory::CreateTestHunter2(size_t player, int x, int y) {
	Unit* unit = new Unit(10, 1, 50 + rand() % 20, 0.5, texture_manager_, game_map_);
	unit->SetTexture(2, 2, 2, 10, 10, 3, 2, 10, 14, 16, 16);
	unit->SetType(UnitType::Ground);
	unit->SetPlayer(player);
	unit->SetPlayersInfo(players_info_);
	BehaviorHunter* inner_behavior = new BehaviorHunter(unit, this);
	//BehaviorAroundPoint* behavior = new BehaviorAroundPoint(unit, this, inner_behavior, x, y, 80);
	unit->SetBehavior(inner_behavior);

	unit->SetPosition(x, y);
	unit->SetCommandPoint(x - 800, y);
	players_[player]->AddUnit(unit);
	return unit;
}

AbstractUnit* UnitFactory::CreateFireSmallPoleax(size_t player, int x, int y) {
	Unit* unit = new Unit(15, 1, 50 + rand() % 20, 0.7, texture_manager_, game_map_);
	unit->SetTexture(TextureName::fire_small_poleax, 2, 5, 10, 3, 7, 10, 10, 14, 24, 24);
	unit->SetType(UnitType::Ground);
	unit->SetPlayer(player);
	unit->SetPlayersInfo(players_info_);
	BehaviorHunter* inner_behavior = new BehaviorHunter(unit, this);
	unit->SetBehavior(inner_behavior);

	unit->SetPosition(x, y);
	unit->SetCommandPoint(x + 800, y);
	players_[player]->AddUnit(unit);
	return unit;
}

AbstractUnit* UnitFactory::CreateFireSmallSpear(size_t player, int x, int y) {
	Unit* unit = new Unit(17, 1, 50 + rand() % 20, 0.7, texture_manager_, game_map_);
	unit->SetTexture(TextureName::fire_small_spear, 2, 4, 10, 5, 7, 10, 10, 14, 24, 24);
	unit->SetType(UnitType::Ground);
	unit->SetPlayer(player);
	unit->SetPlayersInfo(players_info_);
	BehaviorHunter* inner_behavior = new BehaviorHunter(unit, this);
	unit->SetBehavior(inner_behavior);

	unit->SetPosition(x, y);
	unit->SetCommandPoint(x + 800, y);
	players_[player]->AddUnit(unit);
	return unit;
}

AbstractUnit* UnitFactory::CreateFireSmallLance(size_t player, int x, int y) {
	Unit* unit = new Unit(14, 5, 70, 0.7, texture_manager_, game_map_);
	unit->SetTexture(TextureName::fire_small_lance, 2, 4, 10, 4, 7, 10, 10, 14, 24, 24);
	unit->SetType(UnitType::Ground);
	unit->SetPlayer(player);
	unit->SetPlayersInfo(players_info_);
	BehaviorHunter* inner_behavior = new BehaviorHunter(unit, this);
	unit->SetBehavior(inner_behavior);

	unit->SetPosition(x, y);
	unit->SetCommandPoint(x + 800, y);
	players_[player]->AddUnit(unit);
	return unit;
}

AbstractUnit* UnitFactory::CreateFireMediumPoleax(size_t player, int x, int y) {
	Unit* unit = new Unit(30, 5, 150, 0.85, texture_manager_, game_map_);
	unit->SetTexture(TextureName::fire_medium_poleax, 4, 6, 5, 3, 11, 16, 10, 16, 32, 32);
	unit->SetType(UnitType::Ground);
	unit->SetPlayer(player);
	unit->SetPlayersInfo(players_info_);
	BehaviorHunter* inner_behavior = new BehaviorHunter(unit, this);
	unit->SetBehavior(inner_behavior);

	unit->SetPosition(x, y);
	unit->SetCommandPoint(x + 800, y);
	players_[player]->AddUnit(unit);
	return unit;
}

AbstractUnit* UnitFactory::CreateBulletFire1(size_t player, int x, int y, int x_to, int y_to) {
	Unit* unit = new Unit(10, 1, 1, 5.0, texture_manager_, game_map_);
	unit->SetTexture(3, 0, 0, 1, 1, 0, 0, 16, 16, 16, 16);
	unit->SetType(UnitType::Bullet);
	unit->SetPosition(x, y);
	unit->SetPlayer(player);
	unit->SetPlayersInfo(players_info_);
	BehaviorBullet* behavior = new BehaviorBullet(unit, x_to, y_to);
	unit->SetBehavior(behavior);
	players_[player]->AddUnit(unit);
	return unit;
}
