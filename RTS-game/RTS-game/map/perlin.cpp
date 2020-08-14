#include "perlin.h"

#include <array>
#include <cassert>
#include <cmath>

#include "SDL.h"

inline float perlin::lerp(float a0, float a1, float w) {
    return (1.0f - w) * a0 + w * a1;
}

inline float perlin::CalculateDot(SDL_FPoint point, Point grid_point,
    const Grid<SDL_FPoint>& gradients) {
    SDL_FPoint distance = { point.x - grid_point.x, point.y - grid_point.y };
    SDL_FPoint gradient = gradients[grid_point];
    return distance.x * gradient.x + distance.y * gradient.y;
}

inline SDL_FPoint perlin::GetVector() {
    const float PI = acosf(-1.0);
    float angle = static_cast<float>(rand()) * 2.0f * PI / RAND_MAX;
    return { cosf(angle), sinf(angle) };
}

Grid<float> perlin::GetHeights(uint32_t height, uint32_t width) {
    // 1. Make random gradient vectors
    const uint32_t CELL_SIZE = 20;
    size_t gradients_height = static_cast<size_t>(height / CELL_SIZE) + 2;
    size_t gradients_width = static_cast<size_t>(width / CELL_SIZE) + 2;
    Grid<SDL_FPoint> gradients(gradients_height, gradients_width);
    for (size_t i = 0; i < gradients.size(); ++i) {
        for (size_t j = 0; j < gradients[i].size(); ++j) {
            gradients[i][j] = GetVector();
        }
    }

    // 2. Go through each point and compute dot products
    Grid<std::array<float, 4>> dot_products(height, width);
    // 0--1
    // |  |
    // 2--3
    for (uint32_t i = 0; i < height; ++i) {
        for (uint32_t j = 0; j < width; ++j) {
            SDL_FPoint point = { static_cast<float>(j) / CELL_SIZE,
                                 static_cast<float>(i) / CELL_SIZE };
            if (i % CELL_SIZE == 0 && j % CELL_SIZE == 0) {
                dot_products[i][j] = { 0, 0, 0, 0 };
                continue;
            }
            uint32_t left_upper_x = j / CELL_SIZE, left_upper_y = i / CELL_SIZE;
            dot_products[i][j][0] = CalculateDot(point, { left_upper_x, left_upper_y }, gradients);
            dot_products[i][j][1] = CalculateDot(point, { left_upper_x + 1, left_upper_y }, gradients);
            dot_products[i][j][2] = CalculateDot(point, { left_upper_x, left_upper_y + 1 }, gradients);
            dot_products[i][j][3] = CalculateDot(point, { left_upper_x + 1, left_upper_y + 1 }, gradients);
        }
    }

    // 3. Interpolate results
    Grid<float> result(height, width);
    for (uint32_t i = 0; i < height; ++i) {
        for (uint32_t j = 0; j < width; ++j) {
            SDL_FPoint point = { static_cast<float>(j) / CELL_SIZE,
                                 static_cast<float>(i) / CELL_SIZE };
            float weight_x = point.x - (j / CELL_SIZE);
            float weight_y = point.y - (i / CELL_SIZE);
            float result1 = lerp(dot_products[i][j][0], dot_products[i][j][1], weight_x);
            float result2 = lerp(dot_products[i][j][2], dot_products[i][j][3], weight_x);
            result[i][j] = -lerp(result1, result2, weight_y);
        }
    }
    return result;
}
