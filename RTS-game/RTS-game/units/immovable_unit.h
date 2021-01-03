#pragma once

#include "abstract_immovable_unit.h"

#include "../map/game_map.h"
#include "../managers/texture_manager.h"

#include <algorithm>
#include <bitset>
#include <memory>

#include "../clock.h"

#include "../behaviors/status_effects.h"
#include "../behaviors/behavior.h"

#include "../player/players_info.h"

/* Represents an immovable unit on the map
* Unlike AbstractImmovableUnit, it's an abstract class that implements some methods
*/
class ImmovableUnit : public AbstractImmovableUnit {
protected:
	// --base parameters--
	bool alive_ = true;
	UnitType type_ = UnitType::Ground;
	int attack_ = 0, defense_ = 0, health_ = 0, max_health_ = 0;
	std::bitset<EFFECT_SIZE> effects_;
	Behavior* behavior_ = NULL;
	size_t player_ = 0; // player number	
	PlayersInfo* players_info_ = NULL;

	GameMap* game_map_ = NULL;
	uint8_t cur_layer_ind_;

	Clock update_fog_of_war_{ MapLayer::GetBlockSize() };

	// All cells within that radius will be uncovered
	int scout_radius_ = 10 * MapLayer::GetBlockSize();

	void InsertUnitToMap();
	void DeleteUnitFromMap();
	bool CanMoveOnBlock(uint32_t x, uint32_t y);

	//void MapCollisionCheck();
public:
	virtual ~ImmovableUnit();

	virtual int GetX() override;
	virtual int GetY() override;
	int GetCenterX() override;
	int GetCenterY() override;
	int GetAttack() override;
	void GetHitbox(double& x1, double& y1, double& x2, double& y2) override;
	int GetLegsY() override;
	void GetCollisionbox(double& x1, double& y1, double& x2, double& y2) override;

	MapLayer* GetCurrentLayer();

	virtual void GetVector(double& dx, double& dy) override;
	virtual void AddVector(double dx, double dy) override;
	virtual void SetPosition(int x, int y) override;
	virtual void SetPosition(double x, double y) override;

	void DamageApply(int damage) override;
	void UncoverNearbyCells();  // Every cell within scout_radius_ will be uncovered
	void AttackEnd() override;
	virtual void UnitCollide(AbstractImmovableUnit* unit) override;

	void DoAction() override;

	AbstractImmovableUnit* GetClosestUnit(AbstractImmovableUnit* unit1, AbstractImmovableUnit* unit2) override;
	// return first enemy unit in radius
	AbstractImmovableUnit* FindEnemyInRadius(int radius) override;
	// return first enemy unit in block
	AbstractImmovableUnit* GetEnemyInPoint(int x, int y) override;

	void AddEffect(Effect effect) override;
	void RemoveEffect(Effect effect) override;
	bool HasEffect(Effect effect) const override;

	void SetType(UnitType type) override;
	void SetBehavior(Behavior* behavior) override;
	void SetPlayer(size_t player) override;
	void SetPlayersInfo(PlayersInfo* players_info) override;
	size_t GetPlayer() override;

	// check for dead units
	void DeadCheck() override;
	bool IsAlive() override;
	void Die() override;
};