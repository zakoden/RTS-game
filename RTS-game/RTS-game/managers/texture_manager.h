#pragma once

#include <vector>
#include <string>
#include <iostream>

#include "SDL.h"

enum TextureName {
	testunit,
    testunit1,
	testunit2,
	testunit3,
	testunit2_1,
	fire_small_poleax,
	fire_small_spear,
	fire_small_lance,
	fire_medium_poleax
};

class TextureManager {
protected:
	SDL_Renderer* renderer_ = NULL;
	std::vector<SDL_Texture*> textures_;

	void LoadTexture(const char* path);
public:
	TextureManager(SDL_Renderer* renderer);
	~TextureManager();

	SDL_Texture* GetTexture(size_t ind);

};