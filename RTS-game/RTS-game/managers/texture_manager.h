#pragma once

#include <vector>
#include <string>
#include <iostream>

#include "SDL.h"


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