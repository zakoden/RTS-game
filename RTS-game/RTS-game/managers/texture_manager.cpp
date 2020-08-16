#include "texture_manager.h"

void TextureManager::LoadTexture(const char* path) {
	SDL_Surface* surface;
	SDL_Texture* texture;
	surface = SDL_LoadBMP(path);
	SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF));
	texture = SDL_CreateTextureFromSurface(renderer_, surface);
	SDL_FreeSurface(surface);
	textures_.push_back(texture);
}

TextureManager::TextureManager(SDL_Renderer* renderer) {
	renderer_ = renderer;
	
	SDL_Surface* surface; 
	SDL_Texture* texture;

	LoadTexture("pictures/testunit.bmp");
	LoadTexture("pictures/testunit1.bmp");
	LoadTexture("pictures/testunit2.bmp");
	LoadTexture("pictures/testunit3.bmp");
	LoadTexture("pictures/testunit2_1.bmp");
	LoadTexture("pictures/fire_small_poleax.bmp");
	LoadTexture("pictures/fire_small_spear.bmp");
	LoadTexture("pictures/fire_small_lance.bmp");
	LoadTexture("pictures/fire_medium_poleax.bmp");

	surface = NULL;
	texture = NULL;
}

TextureManager::~TextureManager() {
}


SDL_Texture* TextureManager::GetTexture(size_t ind) {
	return textures_[ind];
}