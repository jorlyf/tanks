#include <fstream>
#include "Map.h"
#include "MapBlock.h"
#include "FolderPathManager.h"

void Map::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	for (auto it = _blocks.cbegin(); it != _blocks.cend(); it++)
	{
		MapBlock* block = it->second.get();
		if (block == nullptr) continue;
		target.draw(*block);
	}
}

Map::Map(const std::string& name)
{
	_name = name;
}

void Map::load()
{
	if (_loaded)
	{
		throw new std::exception("Map already loaded.");
	}

	MapState state;
	const std::string filePath = FolderPathManager::mapFolderPath + _name + ".map";

	if (isFileExist(filePath))
	{
		MapState::readFromFile(filePath, &state);
		setMapState(state);
	}

	_loaded = true;
}

void Map::save() const
{
	const MapState state = getMapState();
	const std::string filePath = FolderPathManager::mapFolderPath + _name + ".map";
	MapState::writeToFile(filePath, state);
}

void Map::update()
{
	for (auto it = _blocks.cbegin(); it != _blocks.cend(); it++)
	{
		MapBlock* block = it->second.get();
		if (block == nullptr) continue;
		block->update();
	}
}

std::map<sf::Vector2i, std::unique_ptr<MapBlock>, Vector2iComparator>* Map::getBlocks()
{
	return &_blocks;
}

void Map::addBlock(const MapBlockState& blockState)
{
	_blocks[blockState.position] = std::make_unique<MapBlock>(blockState);
}

void Map::removeBlockAt(const sf::Vector2i& position)
{
	_blocks[position].release();
	_blocks[position] = nullptr;
}

MapState Map::getMapState() const
{
	MapState state;
	for (auto it = _blocks.cbegin(); it != _blocks.cend(); it++)
	{
		const MapBlock* block = it->second.get();
		if (block == nullptr) continue;
		const MapBlockState blockState{ block->getType(), block->getPosition(), block->getHealth() };
		state.blocks.push_back(blockState);
	}
	return state;
}

void Map::setMapState(const MapState& state)
{
	for (auto it = state.blocks.cbegin(); it != state.blocks.cend(); it++)
	{
		const MapBlockState& blockState = *it;
		addBlock(blockState);
	}
}
