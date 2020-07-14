#include "SDL.h"

#include <cassert>
#include <vector>
#include <iostream>

#include "units/unit.h"
#include "units/unit_factory.h"

const size_t kScreenWidth = 800;
const size_t kScreenHeight = 600;

SDL_Window *gWindow = NULL;
SDL_Surface *gSurface = NULL;
SDL_Renderer* gRenderer = NULL;

class Image {
private:
	SDL_Surface* image_ = NULL;

public:
	Image(const std::string& file_path) {
		image_ = SDL_LoadBMP(file_path.c_str());
	}

	~Image() {
		SDL_FreeSurface(image_);
		image_ = NULL;
	}

	operator SDL_Surface* () const { return image_; }
};

bool Init() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cerr << "SDL wasn't initialized! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	gWindow = SDL_CreateWindow("RTS-game",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		kScreenWidth,
		kScreenHeight,
		SDL_WINDOW_SHOWN
	);
	if (gWindow == NULL) {
		std::cerr << "Window wasn't created! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	gSurface = SDL_GetWindowSurface(gWindow);
	if (gSurface == NULL) {
		std::cerr << "Surface wasn't created! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
	if (gRenderer == NULL) {
		std::cerr << "Renderer wasn't created! SDL_Error: " << SDL_GetError() << std::endl;
		return false;
	}

	return true;
}
 
int main(int argc, char *argv[]) {
	Init();
	UnitFactory au(gRenderer);
	au.MakeFireSmall().Move({ 0, 0 });
	au.MakeFireSmall().Draw();
	Unit u1 = au.MakeFireSmall(), u2 = au.MakeFireSmall();
	u1.Attack(u2);
	u2.RemoveEffect(ON_FIRE);
	
	std::vector<int> d;

	Image fire{"assets/fire.bmp"};
	SDL_UpdateWindowSurface(gWindow);

	SDL_Event event_handler;
	for (bool quit = false; !quit; ) {
		while (SDL_PollEvent(&event_handler) != 0) {
			if (event_handler.type == SDL_QUIT) {
				quit = true;
			}

			if (event_handler.type == SDL_KEYDOWN) {
				switch (event_handler.key.keysym.sym) {
					case SDLK_UP:
					case SDLK_DOWN:
					case SDLK_LEFT:
					case SDLK_RIGHT:
						SDL_BlitSurface(fire, NULL, gSurface, NULL);
						SDL_UpdateWindowSurface(gWindow);
					default:
						break;
				}
			}

		}
	}
	
	SDL_DestroyWindow(gWindow);
	return 0;
}
