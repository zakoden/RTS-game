#pragma once

#include "drawable_entity.h"

#include "../behaviors/behavior.h"
#include "../player/players_info.h"
#include "../behaviors/status_effects.h"
#include "unit_type.h"

/* Represents an object on the map
* Interface for ImmovableUnit
* Unlike Entity, it has collisionbox and hitbox; hp, attack and belongs to some player
*/
class AbstractImmovableUnit : public DrawableEntity {
public: 
	virtual ~AbstractImmovableUnit() = default;
	virtual int GetX() = 0;
	virtual int GetY() = 0;
	virtual int GetCenterX() = 0;
	virtual int GetCenterY() = 0;
	virtual int GetAttack() = 0;
	virtual void GetHitbox(double& x1, double& y1, double& x2, double& y2) = 0;
	virtual int GetLegsY() = 0;
	virtual void GetCollisionbox(double& x1, double& y1, double& x2, double& y2) = 0;

	virtual void GetVector(double& dx, double& dy) = 0;
	virtual void AddVector(double dx, double dy) = 0;
	virtual void SetPosition(int x, int y) = 0;
	virtual void SetPosition(double x, double y) = 0;

	virtual void DamageApply(int damage) = 0;
	virtual void AttackEnd() = 0;

	virtual AbstractImmovableUnit* GetClosestUnit(AbstractImmovableUnit* unit1, AbstractImmovableUnit* unit2) = 0;
	// return first enemy unit in radius
	virtual AbstractImmovableUnit* FindEnemyInRadius(int radius) = 0;
	// return first enemy unit in block
	virtual AbstractImmovableUnit* GetEnemyInPoint(int x, int y) = 0;
	virtual void UnitCollide(AbstractImmovableUnit* unit) = 0;

	virtual void DoAction() = 0;
	virtual void SetType(UnitType type) = 0;
	virtual void SetBehavior(Behavior * behavior) = 0;
	virtual void SetPlayer(size_t player) = 0;
	virtual void SetPlayersInfo(PlayersInfo * players_info) = 0;
	virtual size_t GetPlayer() = 0;

	virtual void AddEffect(Effect effect) = 0;
	virtual void RemoveEffect(Effect effect) = 0;
	virtual bool HasEffect(Effect effect) const = 0;

	virtual void DeadCheck() = 0;
	virtual bool IsAlive() = 0;
	virtual void Die() = 0;
};