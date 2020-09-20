#pragma once

#include "map_object.h"

#include <bitset>
#include <cmath>
#include <memory>

#include <iostream>

#include "SDL.h"

#include "unit_type.h"

#include "../map/camera.h"

#include "../behaviors/behavior.h"
#include "../behaviors/status_effects.h"

#include "../player/players_info.h"

class AbstractUnit : MapObject {
public:
	virtual ~AbstractUnit() = default;
	virtual int GetX() = 0;
	virtual int GetY() = 0;
	virtual int GetCenterX() = 0;
	virtual int GetCenterY() = 0;
	virtual int GetSpeed() = 0;
	virtual int GetAttack() = 0;
	virtual void GetHitbox(double& x1, double& y1, double& x2, double& y2) = 0;
	virtual int GetLegsY() = 0;
	virtual void GetCollisionbox(double& x1, double& y1, double& x2, double& y2) = 0;
	virtual void GetVector(double& dx, double& dy) = 0;

	virtual void SetPosition(int x, int y) = 0;
	virtual void SetPosition(double x, double y) = 0;
	virtual void SetVector(int dx, int dy) = 0;
	virtual void SetVector(double dx, double dy) = 0;
	virtual void AddVector(double dx, double dy) = 0;
	virtual void VectorApply() = 0;
	virtual void VectorApplyBullet() = 0;
	virtual void SetCommandPoint(int x, int y) = 0;
	virtual void GetCommandPoint(int& x, int& y) = 0;
	virtual void DamageApply(int damage) = 0;
	virtual void AttackEnd() = 0;
	virtual void UnitCollide(AbstractUnit* unit) = 0;
	
	virtual void DoAction() = 0; 
	virtual void Move() = 0;
	virtual void Draw(SDL_Renderer* renderer, Camera* camera) = 0;

	virtual AbstractUnit* GetClosestUnit(AbstractUnit* unit1, AbstractUnit* unit2) = 0;
	virtual AbstractUnit* FindEnemyInRadius(int radius) = 0;
	virtual AbstractUnit* GetEnemyInPoint(int x, int y) = 0;

	virtual void SetTexture(size_t texture_ind, size_t move_cnt, size_t attack_cnt, 
		                    size_t texture_delay_move, size_t texture_delay_attack,
		                    size_t deltaX, size_t deltaY, size_t width, size_t height,
		                    size_t texture_width, size_t texture_height) = 0;
	virtual void SetType(UnitType type) = 0;
	virtual void SetBehavior(Behavior* behavior) = 0;   
	virtual void SetPlayer(size_t player) = 0;
	virtual void SetPlayersInfo(PlayersInfo* players_info) = 0;
	virtual size_t GetPlayer() = 0;

	virtual void AddEffect(Effect effect) = 0;		 
	virtual void RemoveEffect(Effect effect) = 0;	  
	virtual bool HasEffect(Effect effect) const = 0;  

	virtual void DeadCheck() = 0;
	virtual bool IsAlive() = 0;
	virtual void Die() = 0;
};