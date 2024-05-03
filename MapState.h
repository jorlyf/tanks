#pragma once
#include <vector>
#include <string>
#include <SFML/System/Vector2.hpp>
#include "MapBlockType.h"

struct MapBlockState
{
	MapBlockType type;
	sf::Vector2i position;
	float health;
};

class MapState
{
public:
	std::vector<MapBlockState> blocks;

	static void readFromFile(const std::string& filePath, MapState* state);
	static void writeToFile(const std::string& filePath, const MapState& state);
};
