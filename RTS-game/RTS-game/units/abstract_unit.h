#pragma once

#include <bitset>
#include <cmath>
#include <memory>

#include <iostream>

#include "SDL.h"

#include "../camera.h"

#include "../behaviors/behavior.h"
#include "../behaviors/status_effects.h"

class AbstractUnit {
public:

	virtual int GetX() = 0;
	virtual int GetY() = 0;
	virtual void SetPosition(int x, int y) = 0;
	virtual void SetVector(int dx, int dy) = 0;
	virtual void VectorApply() = 0;
	
	virtual void DoAction() = 0; 

	virtual void Draw(SDL_Renderer* renderer, Camera* camera) const = 0; 

	virtual void SetBehavior(Behavior* behavior) = 0;   
	virtual void SetPlayer(size_t player) = 0;

	virtual void AddEffect(Effect effect) = 0;		 
	virtual void RemoveEffect(Effect effect) = 0;	  
	virtual bool HasEffect(Effect effect) const = 0;  
};