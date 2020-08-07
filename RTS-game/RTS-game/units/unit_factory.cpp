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
	unit->SetTexture(2, 2, 0, 10, 3, 2, 10, 14);
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
	unit->SetTexture(1, 0, 0, 1, 0, 0, 16, 16);
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
	Unit* unit = new Unit(10, 1, 35, 1.0, texture_manager_, game_map_);
	unit->SetTexture(4, 2, 2, 10, 3, 2, 10, 14);
	unit->SetType(UnitType::Ground);
	unit->SetPlayer(player);
	unit->SetPlayersInfo(players_info_);
	BehaviorHunter* behavior = new BehaviorHunter(unit, this);
	unit->SetBehavior(behavior);

	unit->SetPosition(x, y);
	players_[player]->AddUnit(unit);
	return unit;
}

AbstractUnit* UnitFactory::CreateBulletFire1(size_t player, int x, int y, int x_to, int y_to) {
	Unit* unit = new Unit(10, 1, 1, 5.0, texture_manager_, game_map_);
	unit->SetTexture(3, 0, 0, 1, 0, 0, 16, 16);
	unit->SetType(UnitType::Bullet);
	unit->SetPosition(x, y);
	unit->SetPlayer(player);
	unit->SetPlayersInfo(players_info_);
	BehaviorBullet* behavior = new BehaviorBullet(unit, x_to, y_to);
	unit->SetBehavior(behavior);
	players_[player]->AddUnit(unit);
	return unit;
}
