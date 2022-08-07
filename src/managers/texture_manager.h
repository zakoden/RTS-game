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
	fire_medium_poleax,
	small_horizontal_gray_wall,
    small_vertical_gray_wall,
    small_gray_tower,
	bamboo,
	texture_count
};

class TextureManager {
protected:
	SDL_Renderer* renderer_ = NULL;
	std::vector<SDL_Texture*> textures_;

	SDL_Texture* LoadTexture(const std::string& texture_name);
public:
	TextureManager(SDL_Renderer* renderer);
	~TextureManager();

	SDL_Texture* GetTexture(size_t ind);

};