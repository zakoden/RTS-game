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
	SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
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

	const int PLAYERS_COUNT = 2;
	camera_ = new Camera();
	camera_->MoveTo(300, 200);
	game_map_ = new GameMap(renderer_, 200, 200, PLAYERS_COUNT);
	game_map_->Generate();
	texture_manager_ = new TextureManager(renderer_);
	players_info_ = new PlayersInfo(PLAYERS_COUNT);
	unit_factory_ = new UnitFactory();
	unit_factory_->SetMap(game_map_);
	unit_factory_->SetTextureManager(texture_manager_);
	unit_factory_->SetPlayersInfo(players_info_);
	for (size_t i = 0; i < PLAYERS_COUNT; ++i)
		players_info_->SetStatus(i, i, PlayersStatus::PEACE);
	user_manager_ = new UserManager(game_map_, camera_);

	return 0;
}

void GameManager::Run() {

	Player* player0 = new Player(0);
	user_manager_->SetPlayer(player0);
	Player* player1 = new Player(1);
	player0->SetOwner(user_manager_);
	player1->SetOwner(user_manager_);
	players_.push_back(player0);
	players_.push_back(player1);

	for (size_t i = 0; i < players_.size(); ++i) {
		unit_factory_->AddPlayer(players_[i]);
	}

	unit_factory_->CreateBase(0, 200, 200);
	unit_factory_->CreateBase(1, 1200, 200);
	unit_factory_->CreateScout(0, 200, 800, { 100, 100 });
	unit_factory_->CreateScout(1, 1200, 800, { 180, 100 });


	int number = 15;
	for (int i = 0; i < number; ++i) {
		unit_factory_->CreateFireSmallLance(0, 95 + rand() % 10, 100 + 16 * i);
		if (rand() % 2) {
			unit_factory_->CreateFireSmallSpear(0, 130 + rand() % 20, 87 + 16 * i);
		}
		else {
			unit_factory_->CreateFireSmallPoleax(0, 130 + rand() % 20, 87 + 16 * i);
		}
	}

	for (int i = 0; i < number; ++i) {
		unit_factory_->CreateTestHunter2(1, 1000 + rand() % 10, 100 + 18 * i);
		unit_factory_->CreateTestHunter2(1, 1300 + rand() % 10, 95 + 18 * i);
		unit_factory_->CreateTestHunter2(1, 1500 + rand() % 10, 86 + 18 * i);
	}

	unit_factory_->CreateFireSmallPoleax(0, 500 + rand() % 10, 200);
	unit_factory_->CreateFireSmallSpear(0, 500 + rand() % 10, 220);
	unit_factory_->CreateFireSmallLance(0, 500 + rand() % 10, 240);
	unit_factory_->CreateFireMediumPoleax(0, 500 + rand() % 10, 260);

	for (int i = 0; i < 7; ++i) {
		for (int j = 0; j < 7; ++j) {
			entities_.push_back(unit_factory_->CreateBamboo(500 + j * 20 + rand() % 10, 200 + i * 10 + rand() % 5));
		}
	}

	int castle_l = 700;
	int castle_u = 200;
	// *--*
	// |  |
 	//    |
	// |
	// *--*

	// *--*
	unit_factory_->CreateSmallGrayTower(0, castle_l, castle_u);
	unit_factory_->CreateSmallHorizontalGrayWall(0, castle_l + 6, castle_u + 1);
	unit_factory_->CreateSmallHorizontalGrayWall(0, castle_l + 6 + 24, castle_u + 1);
	unit_factory_->CreateSmallGrayTower(0, castle_l + 6 + 24 + 24, castle_u);

	unit_factory_->CreateSmallVerticalGrayWall(0, castle_l + 1, castle_u + 6);
	unit_factory_->CreateSmallVerticalGrayWall(0, castle_l + 1, castle_u + 6 + 18 + 18);

	unit_factory_->CreateSmallVerticalGrayWall(0, castle_l + 6 + 24 + 24 + 1, castle_u + 6);
	unit_factory_->CreateSmallVerticalGrayWall(0, castle_l + 6 + 24 + 24 + 1, castle_u + 6 + 18);

	// *--*
	unit_factory_->CreateSmallGrayTower(0, castle_l, castle_u + 6 + 18 * 3);
	unit_factory_->CreateSmallHorizontalGrayWall(0, castle_l + 6, castle_u + 6 + 18 * 3 + 1);
	unit_factory_->CreateSmallHorizontalGrayWall(0, castle_l + 6 + 24, castle_u + 6 + 18 * 3 + 1);
	unit_factory_->CreateSmallGrayTower(0, castle_l + 6 + 24 + 24, castle_u + 6 + 18 * 3);

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

	game_map_->DrawToTexture(renderer_);
	
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
			time = (static_cast<long long>(time2) - time1) / 1000.0;
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
		case SDL_KEYDOWN: {
			switch (event.key.keysym.sym) {
			case SDLK_F10:
				fog_of_war_mode_ = static_cast<FogOfWarType>((fog_of_war_mode_ + 1) % 3);
				break;

			case SDLK_F11:
				if (is_fullscreen_)
					SDL_SetWindowFullscreen(window_, 0);
				else
					SDL_SetWindowFullscreen(window_, SDL_WINDOW_FULLSCREEN_DESKTOP);
				is_fullscreen_ = !is_fullscreen_;
				break;

			case SDLK_TAB:
				user_manager_->SetPlayer(players_[1 - user_manager_->GetPlayer()->GetNum()]);
			}
			break;
		}

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
		camera_->SetScale(static_cast<float>(scale_status_));
	} else {
		switch (scale_status_) {
		case 0:
			camera_->SetScale(0.8f);
			break;
		case -1:
			camera_->SetScale(0.6f);
			break;
		case -2:
			camera_->SetScale(0.4f);
			break;
		case -3:
			camera_->SetScale(0.2f);
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
	{
		if (fog_of_war_mode_ == VISIBLE)
			game_map_->Draw(renderer_, camera_);
		else
			game_map_->Draw(renderer_, camera_, user_manager_->GetPlayer()->GetNum());

		std::vector<std::pair<int, Drawable*>> units_to_draw;
		for (size_t i = 0; i < players_.size(); ++i) {
			players_[i]->UnitsToDraw(units_to_draw);
		}
		sort(units_to_draw.begin(), units_to_draw.end());
		for (auto& [y, unit] : units_to_draw) {
			unit->Draw(renderer_, camera_);
		}
		
		for (Entity* entity : entities_)
			entity->Draw(renderer_, camera_);


		// TODO optimize it
		/*
		if (fog_of_war_mode_ == UNITS_HIDDEN)
			game_map_->ApplyMask(renderer_, camera_, user_manager_->GetPlayer()->GetNum(), UINT8_MAX - 1);
		else if (fog_of_war_mode_ == MAP_HIDDEN)
			game_map_->ApplyMask(renderer_, camera_, user_manager_->GetPlayer()->GetNum(), UNKNOWN);
		*/
	}


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