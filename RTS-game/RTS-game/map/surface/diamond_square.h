#pragma once

#include "../grid/grid.h"

namespace diamond_square {
	// Returns a number in segment [-1.0, 1.0]
	inline float GetRandomNum();
	inline void CorrectBlock(uint32_t x, uint32_t y, float add, Grid<float>& grid);
	void MakeStep(uint32_t height_, uint32_t width_, uint32_t height, uint32_t width, Grid<float>& grid);

	// Returns a grid with heights, by using diamond-square algorithm
	Grid<float> GetHeights(uint32_t height, uint32_t width);
}