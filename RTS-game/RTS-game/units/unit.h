#pragma once

#include "immovable_unit.h"
#include "movable_unit.h"

#include "../units/abstract_immovable_unit.h"
#include "../map/game_map.h"
#include "../managers/texture_manager.h"

#include <algorithm>
#include <bitset>
#include <memory>

#include "../clock.h"

#include "../behaviors/status_effects.h"
#include "../behaviors/behavior.h"

#include "../player/players_info.h"

class Unit : public MovableUnit {
protected:

	// --geometry--
	double dx_ = 0.0, dy_ = 0.0;
	double speed_ = 0;
	double command_x_ = 0.0, command_y_ = 0.0;

public:
	Unit(int attack, int defense, int max_health, double speed,
		 TextureManager* texture_manager, GameMap* game_map);
	virtual ~Unit() = default;

	virtual int GetSpeed() override;

	virtual void GetVector(double& dx, double& dy) override;
	virtual void SetPosition(int x, int y) override;
	virtual void SetPosition(double x, double y) override;
	virtual void SetVector(int dx, int dy) override;
	virtual void SetVector(double dx, double dy) override;
	virtual void AddVector(double dx, double dy) override;
	virtual void VectorApply() override;
	virtual void VectorApplyBullet() override;
	virtual void SetCommandPoint(int x, int y) override;
	virtual void GetCommandPoint(int& x, int& y) override;

	virtual void Move() override;
	void Draw(SDL_Renderer* renderer, Camera* camera) override;
};