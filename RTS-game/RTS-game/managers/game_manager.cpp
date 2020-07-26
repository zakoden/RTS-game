#include "game_manager.h"

GameManager::GameManager() {
}

GameManager::~GameManager() {

}

int GameManager::Init() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cout << "SDL_Init ERROR : " << SDL_GetError() << std::endl;
		return 1;
	}
	window_ = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (window_ == nullptr) {
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		return 2;
	}
	renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer_ == nullptr) {
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		return 3;
	}
	camera_ = new Camera();
	camera_->MoveTo(300, 200);
	game_map_ = new GameMap(renderer_, 200, 200);
	game_map_->Generate();
	texture_manager_ = new TextureManager(renderer_);
	players_info_ = new PlayersInfo(2);
	unit_factory_ = new UnitFactory();
	unit_factory_->SetMap(game_map_);
	unit_factory_->SetTextureManager(texture_manager_);
	unit_factory_->SetPlayersInfo(players_info_);
	players_info_->SetStatus(0, 0, PlayersStatus::PEACE);
	players_info_->SetStatus(1, 1, PlayersStatus::PEACE);
}

void GameManager::Run() {

	Player* player0 = new Player(0);
	Player* player1 = new Player(1);
	players_.push_back(player0);
	players_.push_back(player1);

	for (size_t i = 0; i < players_.size(); ++i) {
		unit_factory_->AddPlayer(players_[i]);
	}

	unit_factory_->CreateTest(1, 100, 100);
	for (int i = 0; i < 30; ++i) {
		unit_factory_->CreateTest(1, 150 + 10 * i, 150 + rand() % 20);
	}

	unit_factory_->CreateTest1(0, 160, 96);
	for (int i = 0; i < 15; ++i) {
		unit_factory_->CreateTest1(0, 200 + 20 * i, 200 + rand() % 20);
	}
	for (int i = 0; i < 15; ++i) {
		unit_factory_->CreateTest1(0, 200 + 20 * i, 270 + rand() % 20);
	}
	
	while (!close_) {
		RunStep();
	}

	SDL_DestroyRenderer(renderer_);
	SDL_DestroyWindow(window_);
	SDL_Quit();
}

void GameManager::RunStep() {
	
	// events
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			close_ = true;
			break;
		case SDL_MOUSEWHEEL:
			camera_->AddScale(0.1f * event.wheel.y);
			break;
		}
	}

	const Uint8* keyboard_state = SDL_GetKeyboardState(NULL);

	int32_t coef = 10;
	if (keyboard_state[SDL_SCANCODE_UP]) {
		camera_->Move(0, (int32_t)(-coef / camera_->GetScale()));
	}
	if (keyboard_state[SDL_SCANCODE_DOWN]) {
		camera_->Move(0, (int32_t)(coef / camera_->GetScale()));
	}
	if (keyboard_state[SDL_SCANCODE_LEFT]) {
		camera_->Move((int32_t)(-coef / camera_->GetScale()), 0);
	}
	if (keyboard_state[SDL_SCANCODE_RIGHT]) {
		camera_->Move((int32_t)(coef / camera_->GetScale()), 0);
	}

	// clear
	SDL_RenderClear(renderer_);
	SDL_RenderSetScale(renderer_, camera_->GetScale(), camera_->GetScale());

	// move
	for (size_t i = 0; i < players_.size(); ++i) {
		players_[i]->DoAction();
	}
    //unit_->DoAction();

	// draw
	game_map_->Draw(renderer_, camera_);
	for (size_t i = 0; i < players_.size(); ++i) {
		players_[i]->Draw(renderer_, camera_);
	}
	//unit_->Draw(renderer_, camera_);
	SDL_RenderPresent(renderer_);
}