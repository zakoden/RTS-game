#include "players_info.h"

PlayersInfo::PlayersInfo(size_t players) {
	matrix_.resize(players);
	for (size_t i = 0; i < players; ++i) {
		matrix_[i].resize(players);
		for (size_t j = 0; j < players; ++j) {
			matrix_[i][j] = WAR;
		}
	}
}

PlayersInfo::~PlayersInfo() {

}

void PlayersInfo::SetStatus(size_t player1, size_t player2, PlayersStatus status) {
	matrix_[player1][player2] = status;
	matrix_[player2][player1] = status;
}

bool PlayersInfo::CanAttack(size_t player1, size_t player2) {
	return matrix_[player1][player2] == WAR;
}
