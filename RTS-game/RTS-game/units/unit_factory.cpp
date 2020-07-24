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

AbstractUnit* UnitFactory::CreateTest(size_t player, int x, int y) {
	Unit* unit = new Unit(1, 1, 1, 1, 2, texture_manager_, game_map_);
	unit->SetPosition(x, y);
	unit->SetPlayer(player);
	BehaviorRandom* behavior = new BehaviorRandom(unit);
	unit->SetBehavior(behavior);
	players_[player]->AddUnit(unit);
	return unit;
}

AbstractUnit* UnitFactory::CreateTest1(size_t player, int x, int y) {
	Unit* unit = new Unit(1, 1, 1, 1, 1, texture_manager_, game_map_);
	unit->SetPosition(x, y);
	unit->SetPlayer(player);
	BehaviorStay* behavior = new BehaviorStay(unit);
	unit->SetBehavior(behavior);
	players_[player]->AddUnit(unit);
	return unit;
}

AbstractUnit* UnitFactory::CreateBulletFire1(size_t player, int x, int y) {
	//Unit* unit = new Unit(1, 1, 1, 1, NULL, "kek");
	//players_[player]->AddUnit(unit);
	//return unit;
	return NULL;
}
