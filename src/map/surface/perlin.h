#pragma once

#include "SDL.h"

#include "../grid/grid.h"

namespace perlin {
	/* Linear interpolation between a0 and a1
		I've no idea how it works :)
		w should be in range [0, 1]
	*/
	inline float lerp(float a0, float a1, float w);

	// Calculates (gradients[grid_point], point - grid_point)
	inline float CalculateDot(SDL_FPoint point, MapPoint grid_point, const Grid<SDL_FPoint>& gradients);

	// Gets random 2D vector of length 1
	inline SDL_FPoint GetVector();

	// Returns a grid with heights, by using perlin noise
	Grid<float> GetHeights(uint32_t height, uint32_t width);
}