#pragma once

#include "abstract_unit.h"
#include "../map/game_map.h"
#include "../managers/texture_manager.h"

#include <algorithm>
#include <bitset>
#include <memory>

#include "../clock.h"

#include "../behaviors/status_effects.h"
#include "../behaviors/behavior.h"

#include "../player/players_info.h"

class Unit : public AbstractUnit {
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

	Clock update_fog_of_war_{ GameMap::GetBlockSize() };
	
	// All cells within that radius will be uncovered
	int scout_radius_ = 10 * GameMap::GetBlockSize();

	// --cosmetic--
	bool is_right_side = true;
	size_t texture_ind_;
	size_t texture_move_num_ = 0;
	size_t texture_attack_num_ = 0;
	size_t texture_pos_ = 0;
	size_t texture_cur_delay_ = 0;
	size_t texture_delay_move_ = 1;
	size_t texture_delay_attack_ = 1;
	TextureManager* texture_manager_;
	std::string name_;

	// --geometry--
	double x_ = 110.0, y_ = 110.0;
	double dx_ = 0.0, dy_ = 0.0;
	double speed_ = 0;
	double command_x_ = 0.0, command_y_ = 0.0;
	// hitbox delta
	int deltaX_ = 0, deltaY_ = 0;
	// hitbox size
	int width_ = 16, height_ = 16;
	// texture size
	int texture_width_ = 24, texture_height_ = 24;
	/*
	texture
	-----------------------
	|     /|\             |
	|      | deltaY       |
	|     \|/             |
	|      --------       |
	|deltaX|hitbox|       |
	|<---->|      |       |
	|      --------       |
	-----------------------
	*/

    void InsertUnitToMap();
	void DeleteUnitFromMap();
	bool CanMoveOnBlock(uint32_t x, uint32_t y);

	//void MapCollisionCheck();
public:
	Unit(int attack, int defense, int max_health, double speed,
		 TextureManager* texture_manager, GameMap* game_map);
	~Unit() override;

	int GetX() override;
	int GetY() override;
	int GetCenterX() override;
	int GetCenterY() override;
	int GetSpeed() override;
	int GetAttack() override;
	void GetHitbox(double& x1, double& y1, double& x2, double& y2) override;
	int GetLegsY() override;
	void GetCollisionbox(double& x1, double& y1, double& x2, double& y2) override;
	void GetVector(double& dx, double& dy) override;

	void SetPosition(int x, int y) override;
	void SetPosition(double x, double y) override;
	void SetVector(int dx, int dy) override;
	void SetVector(double dx, double dy) override;
	void AddVector(double dx, double dy) override;
    void VectorApply() override;
	void VectorApplyBullet() override;
	void SetCommandPoint(int x, int y) override;
	void GetCommandPoint(int& x, int& y) override;

	void DamageApply(int damage) override;
	void UncoverNearbyCells();  // Every cell within scout_radius_ will be uncovered
	void AttackEnd() override;
	void UnitCollide(AbstractUnit* unit) override;

	void DoAction() override;
	void Move() override;
	void Draw(SDL_Renderer* renderer, Camera* camera) override;

	AbstractUnit* GetClosestUnit(AbstractUnit* unit1, AbstractUnit* unit2) override;
	// return first enemy unit in radius
	AbstractUnit* FindEnemyInRadius(int radius) override;
	// return first enemy unit in block
	AbstractUnit* GetEnemyInPoint(int x, int y) override;

	void AddEffect(Effect effect) override;		  
	void RemoveEffect(Effect effect) override;	  
	bool HasEffect(Effect effect) const override;  

	void SetTexture(size_t texture_ind, size_t move_cnt, size_t attack_cnt, 
		            size_t texture_delay_move, size_t texture_delay_attack,
		            size_t deltaX, size_t deltaY, size_t width, size_t height,
		            size_t texture_width, size_t texture_height) override;
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