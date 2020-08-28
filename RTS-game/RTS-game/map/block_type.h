#pragma once

enum BlockType {
	WATER_DEEP = 0,
	WATER = 1,
	WATER_SHALLOW = 2,
	AMBER = 3,
	GRASS_LIGHT = 4,
	GRASS_OTHER = 5,
	GRASS = 10,
	GRASS_PURPLE = 20,
	ROCK = 30,
	MOUNTAIN_LOW = 40,
	MOUNTAIN_HIGH = 50,
	DESERT_PURPLE = 60,
	DESERT = 70,
	UNKNOWN = 120,

	// For diagram
	GRASSLAND = GRASS,
	SCORCHED = MOUNTAIN_LOW,
	BARE = MOUNTAIN_HIGH,
	TEMPERATE_DESERT = DESERT,

	// Placeholders
	SUBTROPICAL_DESERT = AMBER,
	SHRUBLAND = GRASS_OTHER,
	TEMPERATE_DECIDUOUS_FOREST = GRASS_OTHER,
	TROPICAL_SEASONAL_FOREST = GRASS_LIGHT,
	TROPICAL_RAIN_FOREST = 120,
	TEMPERATE_RAIN_FOREST = GRASSLAND,
	TAIGA = 93,
	TUNDRA = ROCK,
	SNOW = 8
};