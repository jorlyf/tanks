#include <fstream>
#include "MapState.h"

void MapState::readFromFile(const std::string& filePath, MapState* state)
{
	std::ifstream stream(filePath, std::ios_base::in | std::ios_base::binary);

	int blockCount;
	stream.read((char*)&blockCount, sizeof(int));

	for (int i = 0; i < blockCount; i++)
	{
		MapBlockState blockState;
		stream.read((char*)&blockState, sizeof(MapBlockState));
		state->blocks.push_back(blockState);
	}
}

void MapState::writeToFile(const std::string& filePath, const MapState& state)
{
	std::ofstream stream(filePath, std::ios_base::out | std::ios_base::binary);

	int blockCount = state.blocks.size();
	stream.write((char*)&blockCount, sizeof(int));

	for (auto it = state.blocks.cbegin(); it != state.blocks.cend(); it++)
	{
		MapBlockState blockState = *it;
		stream.write((char*)&blockState, sizeof(MapBlockState));
	}
}