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
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

	camera_ = new Camera();
	camera_->MoveTo(300, 200);
	game_map_ = new GameMap(renderer_, 256, 256);
	game_map_->Generate();
	//game_map_->GenerateHeightMap();
	//game_map_->TestGenerate(0);
	texture_manager_ = new TextureManager(renderer_);
	players_info_ = new PlayersInfo(2);
	unit_factory_ = new UnitFactory();
	unit_factory_->SetMap(game_map_);
	unit_factory_->SetTextureManager(texture_manager_);
	unit_factory_->SetPlayersInfo(players_info_);
	players_info_->SetStatus(0, 0, PlayersStatus::PEACE);
	players_info_->SetStatus(1, 1, PlayersStatus::PEACE);
	user_manager_ = new UserManager(game_map_, camera_);
}

void GameManager::Run() {

	Player* player0 = new Player(0);
	user_manager_->AddPlayer(player0);
	Player* player1 = new Player(1);
	player0->SetOwner(user_manager_);
	player1->SetOwner(user_manager_);
	players_.push_back(player0);
	players_.push_back(player1);

	for (size_t i = 0; i < players_.size(); ++i) {
		unit_factory_->AddPlayer(players_[i]);
	}

	int number = 15;
	for (int i = 0; i < number; ++i) {
		unit_factory_->CreateTestHunter(0, 80 + rand() % 10, 92 + 16 * i);
		unit_factory_->CreateTestHunter(0, 95 + rand() % 10, 100 + 16 * i);
		unit_factory_->CreateTestHunter(0, 130 + rand() % 20, 87 + 16 * i);
	}

	for (int i = 0; i < number; ++i) {
		unit_factory_->CreateTestHunter2(1, 1000 + rand() % 10, 100 + 18 * i);
		unit_factory_->CreateTestHunter2(1, 1300 + rand() % 10, 95 + 18 * i);
		unit_factory_->CreateTestHunter2(1, 1500 + rand() % 10, 86 + 18 * i);
	}

	/*
	unit_factory_->CreateTest(1, 150, 100);
	for (int i = 0; i < 30; ++i) {
		unit_factory_->CreateTest(1, 150 + 10 * i, 150 + rand() % 20);
	}

	unit_factory_->CreateTestHunter(0, 40, 40);
	*/

	/*
	unit_factory_->CreateTest1(0, 160, 96);
	for (int i = 0; i < 15; ++i) {
		unit_factory_->CreateTest1(0, 200 + 20 * i, 200 + rand() % 20);
	}
	for (int i = 0; i < 15; ++i) {
		unit_factory_->CreateTest1(0, 200 + 20 * i, 270 + rand() % 20);
	}
	*/

	ground_h_ = 0.0;
	air_h_ = 0.14;
	camera_h_ = 1.0;
	
	int count = 0;
	int time1, time2;
	double time;
	while (!close_) {
		if (count == 0) {
			time1 = SDL_GetTicks();
		}
		RunStep();
		count++;
		if (count == 200) {
			time2 = SDL_GetTicks();
			time = (time2 - time1) / 1000.0;
			std::cout << "FPS : " << (double)count / (double)time << std::endl;
			count = 0;
		}
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
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_F11) {
				if (is_fullscreen_)
					SDL_SetWindowFullscreen(window_, 0);
				else
					SDL_SetWindowFullscreen(window_, SDL_WINDOW_FULLSCREEN_DESKTOP);
				is_fullscreen_ = !is_fullscreen_;
			}
			break;

		case SDL_QUIT:
			close_ = true;
			break;
		case SDL_MOUSEWHEEL:
			if (event.wheel.y < 0) {
				scale_status_--;
			}
			if (event.wheel.y > 0) {
				scale_status_++;
			}
			scale_status_ = std::max(scale_status_, -3);
			scale_status_ = std::min(scale_status_, 20);
			break;
		}
	}
	
    double coef = 10.0;

	int mouse_x, mouse_y;
	uint32_t mouse_state = SDL_GetMouseState(&mouse_x, &mouse_y);
	int mouse_move_size = 20;
	int window_w, window_h;
	SDL_GetWindowSize(window_, &window_w, &window_h);
	if (mouse_x < mouse_move_size) {
		camera_->Move(-coef / camera_->GetScale(), 0);
		camera_->MoveTo(std::max(camera_->GetX(), 0), camera_->GetY());
	}
	if (mouse_x > (window_w - mouse_move_size)) {
		camera_->Move(coef / camera_->GetScale(), 0);
		camera_->MoveTo(std::min(camera_->GetX(), (int32_t)(game_map_->GetWidth() * game_map_->GetBlockSize())), camera_->GetY());
	}
	if (mouse_y < mouse_move_size) {
		camera_->Move(0, -coef / camera_->GetScale());
		camera_->MoveTo(camera_->GetX(), std::max(camera_->GetY(), 0));
	}
	if (mouse_y > (window_h - mouse_move_size)) {
		camera_->Move(0, coef / camera_->GetScale());
		camera_->MoveTo(camera_->GetX(), std::min(camera_->GetY(), (int32_t)(game_map_->GetHeight() * game_map_->GetBlockSize())));
	}

	const Uint8* keyboard_state = SDL_GetKeyboardState(NULL);
	if (keyboard_state[SDL_SCANCODE_UP]) {
		camera_->Move(0, -coef / camera_->GetScale());
	}
	if (keyboard_state[SDL_SCANCODE_DOWN]) {
		camera_->Move(0, coef / camera_->GetScale());
	}
	if (keyboard_state[SDL_SCANCODE_LEFT]) {
		camera_->Move(-coef / camera_->GetScale(), 0);
	}
	if (keyboard_state[SDL_SCANCODE_RIGHT]) {
		camera_->Move(coef / camera_->GetScale(), 0);
	}

	// clear
	SDL_RenderClear(renderer_);
	
	if (scale_status_ >= 1) {
		camera_->SetScale(scale_status_);
	} else {
		switch (scale_status_) {
		case 0:
			camera_->SetScale(0.8);
			break;
		case -1:
			camera_->SetScale(0.6);
			break;
		case -2:
			camera_->SetScale(0.4);
			break;
		case -3:
			camera_->SetScale(0.2);
			break;
		}
	}
	
	//camera_->SetScale(screen_h_ / (camera_h_ - ground_h_));
	SDL_RenderSetScale(renderer_, camera_->GetScale(), camera_->GetScale());

	// move

	user_manager_->DoAction(renderer_);

	for (size_t i = 0; i < players_.size(); ++i) {
		players_[i]->DoAction();
	}

	for (size_t i = 0; i < players_.size(); ++i) {
		players_[i]->Move();
	}


	// draw
	game_map_->Draw(renderer_, camera_);
	for (size_t i = 0; i < players_.size(); ++i) {
		players_[i]->Draw(renderer_, camera_);
	}

	user_manager_->Draw(renderer_, camera_);


	/*
	if (camera_h_ > air_h_) {
		camera_->SetScale(screen_h_ / (camera_h_ - air_h_));
		SDL_RenderSetScale(renderer_, camera_->GetScale(), camera_->GetScale());
		for (size_t i = 0; i < players_.size(); ++i) {
			players_[i]->Draw(renderer_, camera_);
		}
	}
	std::cout << camera_->GetScale() << std::endl;
	*/

	SDL_RenderPresent(renderer_);
}