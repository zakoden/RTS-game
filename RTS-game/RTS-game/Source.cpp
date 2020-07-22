#include <iostream>

#include "SDL.h"

#include "Game_Map.h"
#include "Camera.h"

#include <vector>

int main(int argc, char* argv[]) {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cout << "SDL_Init ERROR : " << SDL_GetError() << std::endl;
		return 1;
	}

	SDL_Window* win = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (win == nullptr) {
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	SDL_Renderer* ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr) {
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		return 1;
	}

	Camera* camera = new Camera();
	camera->MoveTo(300, 200);
	GameMap* game_map = new GameMap(ren, 200, 200);
	game_map->TestGenerate();

	SDL_RenderClear(ren);
	SDL_RenderPresent(ren);



	bool close = false;
	SDL_Event event;
	while (!close)
	{
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				close = true;
				break;
			case SDL_MOUSEWHEEL:
				camera->AddScale(0.1 * event.wheel.y);
				break;
			case SDL_KEYDOWN:
				/*
				int32_t coef = 10;
				switch (event.key.keysym.sym) {
				case SDLK_UP:
					camera->Move(0, (int32_t)(-coef / camera->GetScale()));
					break;
				case SDLK_DOWN:
					camera->Move(0, (int32_t)(coef / camera->GetScale()));
					break;
				case SDLK_LEFT:
					camera->Move((int32_t)(-coef / camera->GetScale()), 0);
					break;
				case SDLK_RIGHT:
					camera->Move((int32_t)(coef / camera->GetScale()), 0);
					break;
				}
				*/
				break;
			}
		}

		const Uint8* keyboard_state = SDL_GetKeyboardState(NULL);

		int32_t coef = 10;
		if (keyboard_state[SDL_SCANCODE_UP]) {
			camera->Move(0, (int32_t)(-coef / camera->GetScale()));
		}
		if (keyboard_state[SDL_SCANCODE_DOWN]) {
			camera->Move(0, (int32_t)(coef / camera->GetScale()));
		}
		if (keyboard_state[SDL_SCANCODE_LEFT]) {
			camera->Move((int32_t)(-coef / camera->GetScale()), 0);
		}
		if (keyboard_state[SDL_SCANCODE_RIGHT]) {
			camera->Move((int32_t)(coef / camera->GetScale()), 0);
		}

		SDL_RenderClear(ren);
		SDL_RenderSetScale(ren, camera->GetScale(), camera->GetScale());

		//std::cout << camera->GetScale() << " " << camera->GetCornerX(ren) << " " << camera->GetCornerY(ren) << std::endl;

		game_map->Draw(ren, camera);
		SDL_RenderPresent(ren);
	}


	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();


	return 0;
}