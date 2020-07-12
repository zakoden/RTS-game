#include <iostream>

#include "SDL.h"

//#include "Game_Map.h"

#include <vector>

int main(int argc, char *argv[]) {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cout << "SDL_Init ERROR : " << SDL_GetError() << std::endl;
		return 1;
	}

	std::vector<int> v;
	/*
	SDL_Window* win = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
	if (win == nullptr) {
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr) {
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	bool close = false;
	SDL_Event event;
	while (!close)
	{
		SDL_WaitEvent(&event);
		switch(event.type)
		{
            case SDL_QUIT: 
				close = true;
		}
	}
	SDL_RenderClear(ren);
	SDL_RenderPresent(ren);

	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	*/

	return 0;
}