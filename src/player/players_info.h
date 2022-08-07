#pragma once

#include <vector>
#include <cstdint>

enum PlayersStatus {
	PEACE,
	WAR
};

class PlayersInfo {
protected:
	// vector<string> race_
	std::vector<std::vector<uint8_t>> matrix_;
public:
	PlayersInfo(size_t players);
	~PlayersInfo();

	void SetStatus(size_t player1, size_t player2, PlayersStatus status);

	bool CanAttack(size_t player1, size_t player2);
};