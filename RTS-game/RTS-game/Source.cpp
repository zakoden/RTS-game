#include <iostream>

#include "managers/game_manager.h"

int main(int argc, char *argv[]) {

	GameManager* game_manager = new GameManager();
	game_manager->Init();
	game_manager->Run();

	return 0;
}
