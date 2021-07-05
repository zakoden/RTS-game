#pragma once

#include "../player/players_info.h"

class WithPlayer {
public:
	virtual ~WithPlayer() = default;

	virtual void SetPlayer(size_t player) = 0;
	virtual void SetPlayersInfo(PlayersInfo* players_info) = 0;
	virtual size_t GetPlayer() = 0;
};