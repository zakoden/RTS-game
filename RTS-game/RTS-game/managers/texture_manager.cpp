#include "texture_manager.h"

inline std::string GetFileName(const std::string& texture_name) {
	return "pictures/" + texture_name + ".bmp";
}

SDL_Texture* TextureManager::LoadTexture(const std::string& texture_name) {
	SDL_Surface* surface = SDL_LoadBMP(GetFileName(texture_name).c_str());
	if (surface == NULL) {
		std::cerr << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
	}

	SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF));
	
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer_, surface);
	SDL_FreeSurface(surface);
	return texture;
}

TextureManager::TextureManager(SDL_Renderer* renderer) {
	renderer_ = renderer;
	
	textures_.resize(texture_count);
	textures_[testunit]				           = LoadTexture("testunit");
	textures_[testunit1]			           = LoadTexture("testunit1");
	textures_[testunit2]			           = LoadTexture("testunit2");
	textures_[testunit3]			           = LoadTexture("testunit3");
	textures_[testunit2_1]			           = LoadTexture("testunit2_1");
	textures_[fire_small_poleax]	           = LoadTexture("fire_small_poleax");
	textures_[fire_small_spear]		           = LoadTexture("fire_small_spear");
	textures_[fire_small_lance]		           = LoadTexture("fire_small_lance");
	textures_[fire_medium_poleax]	           = LoadTexture("fire_medium_poleax");
	textures_[small_horizontal_gray_wall]      = LoadTexture("small_horizontal_gray_wall");
	textures_[small_vertical_gray_wall]        = LoadTexture("small_vertical_gray_wall");
	textures_[small_gray_tower]                = LoadTexture("small_gray_tower");
	textures_[bamboo]                          = LoadTexture("bamboo");
}

TextureManager::~TextureManager() {
}


SDL_Texture* TextureManager::GetTexture(size_t ind) {
	return textures_[ind];
}