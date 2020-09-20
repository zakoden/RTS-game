#include "unit_factory.h"

#include "../behaviors/behavior_random.h"
#include "../behaviors/behavior_tower.h"
#include "../behaviors/behavior_scout.h"
#include "../behaviors/behavior_stay.h"
#include "../behaviors/behavior_bullet.h"
#include "../behaviors/behavior_hunter.h"
#include "../behaviors/behavior_around_point.h"

#include "../units/entity.h"

void UnitFactory::FillUnit(Unit* unit, UnitType unit_type, size_t player, Behavior* behavior, int x, int y) {
	unit->SetType(unit_type);
	unit->SetPlayer(player);
	unit->SetPlayersInfo(players_info_);
	unit->SetBehavior(behavior);

	unit->SetPosition(x, y);
}

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
	FillUnit(unit, UnitType::Ground, player, new BehaviorRandom(unit), x, y);

	players_[player]->AddUnit(unit);
	return unit;
}

AbstractUnit* UnitFactory::CreateTest1(size_t player, int x, int y) {
	Unit* unit = new Unit(10, 1, 35, 1.0, texture_manager_, game_map_);
	unit->SetTexture(1, 0, 0, 1, 1, 0, 0, 16, 16, 16, 16);
	FillUnit(unit, UnitType::Ground, player, new BehaviorTower(unit, this), x, y);

	players_[player]->AddUnit(unit);
	return unit;
}

AbstractUnit* UnitFactory::CreateTestHunter(size_t player, int x, int y) {
	Unit* unit = new Unit(10 + rand() % 5, 1, 40, 1.0, texture_manager_, game_map_);
	unit->SetTexture(4, 2, 2, 10, 10, 3, 2, 10, 14, 16, 16);
	FillUnit(unit, UnitType::Ground, player, new BehaviorHunter(unit, this), x, y);

	unit->SetCommandPoint(x + 800, y);
	players_[player]->AddUnit(unit);
	return unit;
}

AbstractUnit* UnitFactory::CreateTestHunter2(size_t player, int x, int y) {
	Unit* unit = new Unit(10, 1, 50 + rand() % 20, 0.5, texture_manager_, game_map_);
	unit->SetTexture(2, 2, 2, 10, 10, 3, 2, 10, 14, 16, 16);
	FillUnit(unit, UnitType::Ground, player, new BehaviorHunter(unit, this), x, y);

	unit->SetCommandPoint(x - 800, y);
	players_[player]->AddUnit(unit);
	return unit;
}

AbstractUnit* UnitFactory::CreateFireSmallPoleax(size_t player, int x, int y) {
	Unit* unit = new Unit(15, 1, 50 + rand() % 20, 0.7, texture_manager_, game_map_);
	unit->SetTexture(TextureName::fire_small_poleax, 2, 5, 10, 3, 7, 10, 10, 14, 24, 24);
	FillUnit(unit, UnitType::Ground, player, new BehaviorHunter(unit, this), x, y);

	unit->SetCommandPoint(x + 800, y);
	players_[player]->AddUnit(unit);
	return unit;
}

AbstractUnit* UnitFactory::CreateFireSmallSpear(size_t player, int x, int y) {
	Unit* unit = new Unit(17, 1, 50 + rand() % 20, 0.7, texture_manager_, game_map_);
	unit->SetTexture(TextureName::fire_small_spear, 2, 4, 10, 5, 7, 10, 10, 14, 24, 24);
	FillUnit(unit, UnitType::Ground, player, new BehaviorHunter(unit, this), x, y);

	unit->SetCommandPoint(x + 800, y);
	players_[player]->AddUnit(unit);
	return unit;
}

AbstractUnit* UnitFactory::CreateFireSmallLance(size_t player, int x, int y) {
	Unit* unit = new Unit(14, 5, 70, 0.7, texture_manager_, game_map_);
	unit->SetTexture(TextureName::fire_small_lance, 2, 4, 10, 4, 7, 10, 10, 14, 24, 24);
	FillUnit(unit, UnitType::Ground, player, new BehaviorHunter(unit, this), x, y);

	unit->SetCommandPoint(x + 800, y);
	players_[player]->AddUnit(unit);
	return unit;
}

AbstractUnit* UnitFactory::CreateFireMediumPoleax(size_t player, int x, int y) {
	Unit* unit = new Unit(30, 5, 150, 0.85, texture_manager_, game_map_);
	unit->SetTexture(TextureName::fire_medium_poleax, 4, 6, 5, 3, 11, 16, 10, 16, 32, 32);
	FillUnit(unit, UnitType::Ground, player, new BehaviorHunter(unit, this), x, y);

	unit->SetCommandPoint(x + 800, y);
	players_[player]->AddUnit(unit);
	return unit;
}

AbstractUnit * UnitFactory::CreateScout(size_t player, int x, int y, MapPoint center) {
	Unit* unit = new Unit(4, 1, 40 + rand() % 20, 2, texture_manager_, game_map_);
	unit->SetTexture(2, 2, 2, 10, 10, 3, 2, 10, 14, 16, 16);
	FillUnit(unit, UnitType::Ground, player, new BehaviorScout(unit, game_map_, center), x, y);

	unit->SetCommandPoint(x - 800, y);
	players_[player]->AddUnit(unit);
	return unit;
}

AbstractUnit* UnitFactory::CreateBulletFire1(size_t player, int x, int y, int x_to, int y_to) {
	Unit* unit = new Unit(10, 1, 1, 5.0, texture_manager_, game_map_);
	unit->SetTexture(3, 0, 0, 1, 1, 0, 0, 16, 16, 16, 16);
	FillUnit(unit, UnitType::Bullet, player, new BehaviorBullet(unit, x_to, y_to), x, y);

	players_[player]->AddUnit(unit);
	return unit;
}

AbstractUnit* UnitFactory::CreateBase(uint8_t player, int x, int y) {
	Unit* unit = new Unit(10, 10, 100, 0, texture_manager_, game_map_);
	unit->SetTexture(1, 0, 0, 1, 1, 0, 0, 16, 16, 16, 16);
	FillUnit(unit, UnitType::Ground, player, new BehaviorStay(unit), x, y);

	players_[player]->AddUnit(unit);
	game_map_->AddBase({ x, y }, player);
	return unit;
}

// 24 x 4
AbstractUnit* UnitFactory::CreateSmallHorizontalGrayWall(uint8_t player, int x, int y) {
	Unit* unit = new Unit(10, 10, 100, 0, texture_manager_, game_map_);
	unit->SetTexture(TextureName::small_horizontal_gray_wall, 0, 0, 1, 1, 0, 15, 24, 4, 24, 19);
	FillUnit(unit, UnitType::Ground, player, new BehaviorStay(unit), x, y - 15);

	players_[player]->AddUnit(unit);
	return unit;
}


// 4 x 18
AbstractUnit* UnitFactory::CreateSmallVerticalGrayWall(uint8_t player, int x, int y) {
	Unit* unit = new Unit(10, 10, 100, 0, texture_manager_, game_map_);
	unit->SetTexture(TextureName::small_vertical_gray_wall, 0, 0, 1, 1, 0, 17, 4, 18, 4, 35);
	FillUnit(unit, UnitType::Ground, player, new BehaviorStay(unit), x, y - 17);

	players_[player]->AddUnit(unit);
	return unit;
}

// 6 x 6
AbstractUnit* UnitFactory::CreateSmallGrayTower(uint8_t player, int x, int y) {
	Unit* unit = new Unit(10, 10, 100, 0, texture_manager_, game_map_);
	unit->SetTexture(TextureName::small_gray_tower, 0, 0, 1, 1, 0, 20, 6, 6, 6, 26);
	FillUnit(unit, UnitType::Ground, player, new BehaviorStay(unit), x, y - 20);

	players_[player]->AddUnit(unit);
	return unit;
}

Entity* UnitFactory::CreateBamboo(int x, int y) {
	Entity* entity = new Entity(texture_manager_, x, y - 28);
	entity->SetTexture(TextureName::bamboo, 0, 0, 1, 1, 0, 0, 0, 0, 16, 28);

	return entity;
}
