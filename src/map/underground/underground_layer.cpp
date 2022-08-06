#include "underground_layer.h"

#include "../grid/grid_neighbors.h"
#include "../grid/grid_function.h"

#include "../../utils/time_measurer.h"

#include <random>

Grid<char> SimulateAutomaton(const Grid<char>& state, const GridNeighbors& grid_neighbors, std::mt19937& rng) {
	const int BIRTH_LIMIT = 5;
	const int DEATH_LIMIT = 4;
	const int OVERFLOW_LIMIT = 8;

	Grid<char> result = state;
	for (uint32_t row = 0; row < state.size(); ++row) {
		for (uint32_t col = 0; col < state[row].size(); ++col) {
			int alive = 0;
			for (const MapPoint neighbor : grid_neighbors[row][col]) {
				alive += state[neighbor];
			}
			if (state[row][col]) { // Cell is alive
				if (alive < DEATH_LIMIT) {
					result[row][col] = false;
				}
				if (alive >= OVERFLOW_LIMIT) {
					if (rng() % 6 != 0) {
						result[row][col] = false;
					}
				}
			} else {  // Cell is dead
				if (alive >= BIRTH_LIMIT) {
					result[row][col] = true;
				}
			}
		}
	}

	return result;
}

void UndergroundLayer::Generate(uint64_t seed) {
	TimeMeasurer time_measurer, time_total;
	std::cout << "UndergroundLayer::Generate: Started" << std::endl;
	std::mt19937 rng(seed);

	const double START_ALIVE_PERCENT = 90;
	Grid<char> state(height_, width_);
	for (uint32_t row = 0; row < height_; ++row) {
		for (uint32_t col = 0; col < width_; ++col) {
			state[row][col] = rng() % 100 < START_ALIVE_PERCENT;
		}
	}
	GridNeighbors grid_neighbors(state);

	time_measurer.PrintTime("UndergroundLayer::Generate: Started iterations");
	const int ITERATIONS_COUNT = 10;
	for (int i = 0; i < ITERATIONS_COUNT; ++i) {
		state = SimulateAutomaton(state, grid_neighbors, rng);
	}
	time_measurer.PrintTime("UndergroundLayer::Generate: Finished iterations");

	Grid<BlockType> result = grid_function::FromFunction<BlockType>(height_, width_, 
		[&](size_t row, size_t col) { return state[row][col] ? CAVE_FLOOR : CAVE_WALL; }
	);
	grid_function::SmoothMap(grid_neighbors, result);
	grid_function::RemoveSmallAreas(grid_neighbors, result, 25);

	int floor_cells = 0;
	for (uint32_t row = 0; row < height_; ++row) {
		for (uint32_t col = 0; col < width_; ++col) {
			blocks_[GetInd(col, row)] = result[row][col];
			floor_cells += result[row][col] == CAVE_FLOOR;
		}
	}
	std::cout << "UndergroundLayer::Generate: Floor area: " << floor_cells 
		<< " (" << floor_cells * 100 / (height_ * width_) << "%)" <<  std::endl;
	time_total.PrintTime("UndergroundLayer::Generate: Finished");
}
