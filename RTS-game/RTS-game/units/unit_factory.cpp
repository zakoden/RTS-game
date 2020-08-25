#include "unit_factory.h"

#include "../behaviors/behavior_random.h"
#include "../behaviors/behavior_tower.h"
#include "../behaviors/behavior_scout.h"
#include "../behaviors/behavior_stay.h"
#include "../behaviors/behavior_bullet.h"
#include "../behaviors/behavior_hunter.h"
#include "../behaviors/behavior_around_point.h"

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
	Unit* unit = new Unit(10 + rand() % 5, 1, 40, 1.0, texture_manager_, game_map_);
	unit->SetTexture(4, 2, 2, 10, 3, 2, 10, 14);
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
	unit->SetTexture(2, 2, 2, 10, 3, 2, 10, 14);
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

AbstractUnit* UnitFactory::CreateScout(size_t player, int x, int y, MapPoint center) {
	Unit* unit = new Unit(4, 1, 40 + rand() % 20, 2, texture_manager_, game_map_);
	unit->SetTexture(2, 2, 2, 10, 3, 2, 10, 14);
	unit->SetType(UnitType::Ground);
	unit->SetPlayer(player);
	unit->SetPlayersInfo(players_info_);
	BehaviorHunter* inner_behavior = new BehaviorHunter(unit, this);
	//BehaviorAroundPoint* behavior = new BehaviorAroundPoint(unit, this, inner_behavior, x, y, 80);
	unit->SetBehavior(new BehaviorScout(unit, game_map_, center));

	unit->SetPosition(x, y);
	unit->SetCommandPoint(x - 800, y);
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

AbstractUnit* UnitFactory::CreateBase(uint8_t player, int x, int y) {
	Unit* unit = new Unit(10, 10, 100, 0, texture_manager_, game_map_);
	unit->SetTexture(3, 0, 0, 1, 0, 0, 16, 16);
	unit->SetType(UnitType::Ground);
	unit->SetPosition(x, y);
	unit->SetPlayer(player);
	unit->SetPlayersInfo(players_info_);
	unit->SetBehavior(new BehaviorStay(unit));

	players_[player]->AddUnit(unit);
	game_map_->AddBase({ x, y }, player);

	return nullptr;
}
