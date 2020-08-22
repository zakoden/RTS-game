#include "diamond_square.h"

#include <algorithm>

#include "grid_function.h"

inline float diamond_square::GetRandomNum() {
	int len = 10000;
	int val = rand() % len;
	if (rand() % 2) val = -val;
	return (float)val / (float)len;
}

inline void diamond_square::CorrectBlock(uint32_t x, uint32_t y, float add, Grid<float>& grid) {
	float delta = GetRandomNum() * add;
	grid[y][x] = std::clamp(grid[y][x] + delta, -1.0f, 1.0f);
}

void diamond_square::MakeStep(uint32_t height_, uint32_t width_, uint32_t height, uint32_t width, Grid<float>& grid) {
	if (height == 1 || width == 1) return;

	double coef = 4.01 / 128.0;

	// square part
	for (int i = 0; i < height_ / height; ++i) {
		for (int j = 0; j < width_ / width; ++j) {
			int h = height >> 1, w = width >> 1;
			int cx = j * width + w, cy = i * height + h;

			float block = (
				grid[cy - h][cx - w] +
				grid[cy - h][cx + w] +
				grid[cy + h][cx - w] +
				grid[cy + h][cx + w]
				) / 4.0;
			grid[cy][cx] = block;
			CorrectBlock(cx, cy, (h + w) * coef, grid);
		}
	}

	//diamond part
	for (int i = 0; i <= height_ / height; ++i) {
		int h = height >> 1, w = width >> 1, cx, cy;
		for (int j = 0; j < width_ / width; ++j) {
			cx = j * width + w;
			cy = i * height;
			float block = grid[cy][cx + w] + grid[cy][cx - w];
			if ((cy - h) >= 0)
				block += grid[cy - h][cx];
			else
				block += 0.0;
			if ((cy + h) < height_)
				block += grid[cy + h][cx];
			else
				block += 0.0;
			block /= 4.0;

			grid[cy][cx] = block;
			CorrectBlock(cx, cy, (h + w) * coef, grid);
		}
	}
	for (int i = 0; i < height_ / height; ++i) {
		int h, w, cx, cy;
		h = height / 2;
		w = width / 2;
		for (int j = 0; j <= width_ / width; ++j) {
			cx = j * width;
			cy = i * height + h;
			float block;
			block = grid[cy + h][cx] + grid[cy - h][cx];
			if ((cx - w) >= 0)
				block += grid[cy][cx - w];
			else
				block += 0.0;
			if ((cx + w) < width_)
				block += grid[cy][cx + w];
			else
				block += 0.0;
			block /= 4.0;
			grid[cy][cx] = block;
			CorrectBlock(cx, cy, (h + w) * coef, grid);
		}
	}
}


inline bool IsPowerOfTwo(uint32_t a) { return (a & (a - 1)) == 0; }

Grid<float> diamond_square::GetHeights(uint32_t height, uint32_t width) {
	uint32_t required_height = height, required_width = width;
	while (!IsPowerOfTwo(height - 1))
		++height;
	while (!IsPowerOfTwo(width - 1))
		++width;
	Grid<float> grid(height, width, 0.0);

	uint32_t h = height - 1, w = width - 1;
	grid[0][0] = GetRandomNum();
	grid[0][w] = GetRandomNum();
	grid[h][0] = GetRandomNum();
	grid[h][w] = GetRandomNum();

	while (h > 0 && w > 0) {
		MakeStep(height, width, h, w, grid);
		h >>= 1;
		w >>= 1;
	}

	return grid_function::FromFunction<float>(required_height, required_width,
		[&](size_t i, size_t j) { return grid[i][j]; });
}

