#pragma once

#include "abstract_unit.h"
#include "../game_map.h"
#include "../managers/texture_manager.h"

#include <bitset>
#include <memory>

#include "../behaviors/status_effects.h"
#include "../behaviors/behavior.h"

class Unit : public AbstractUnit {
protected:
	// --base parameters--
	int attack_ = 0, defense_ = 0, health_ = 0, max_health_ = 0;
	std::bitset<EFFECT_SIZE> effects_;
	Behavior* behavior_;
	size_t player_ = 0; // player number	
	GameMap* game_map_ = NULL;

	// --cosmetic--
	size_t texture_num_;
	TextureManager* texture_manager_;
	std::string name_;

	// --geometry--
	int x_ = 110, y_ = 110;
	int dx_ = 0, dy_ = 0;
	uint32_t speed_ = 0;
	// hitbox delta
	int deltaX_ = 4, deltaY_ = 4;
	// hitbox size
	int width_ = 8, height_ = 8;
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
public:
    virtual ~Unit() = default;

	Unit(int attack, int defense, int max_health, uint32_t speed,
		 size_t texture_num, 
		 TextureManager* texture_manager, GameMap* game_map);

	int GetX() override;
	int GetY() override;
	void SetPosition(int x, int y) override;
	void SetVector(int dx, int dy) override;
    void VectorApply() override;

	void DoAction() override;
	void Draw(SDL_Renderer* renderer, Camera* camera) const override;

	void AddEffect(Effect effect) override;		  
	void RemoveEffect(Effect effect) override;	  
	bool HasEffect(Effect effect) const override;  

	void SetBehavior(Behavior* behavior) override;
	void SetPlayer(size_t player) override;
};