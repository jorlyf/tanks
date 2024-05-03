#include <iostream>
#include "MapEditorEngine.h"
#include "Map.h"
#include "MapBlock.h"
#include "GhostMapBlock.h"

void MapEditorEngine::processGhostBlockTypeSelecting()
{
	if (WindowInput::getKeyboardKeyState(sf::Keyboard::Key::Num1).isPressed)
	{
		_ghostBlock->setType(MapBlockType::metal);
	}
}

void MapEditorEngine::processGhostBlockPosition()
{
	const sf::Vector2f mouseWorldPosition = WindowInput::getWorldMousePosition();
	const int xOffset = mouseWorldPosition.x >= 0 ? MapBlock::SIZE / 2 : -MapBlock::SIZE / 2;
	const int yOffset = mouseWorldPosition.y >= 0 ? MapBlock::SIZE / 2 : -MapBlock::SIZE / 2;
	sf::Vector2i blockPosition;
	blockPosition.x = (mouseWorldPosition.x + xOffset) / MapBlock::SIZE;
	blockPosition.y = (mouseWorldPosition.y + yOffset) / MapBlock::SIZE;
	_ghostBlock->setPosition(blockPosition);
}

void MapEditorEngine::processGhostBlockPlacing()
{
	if (WindowInput::getMouseButtonState(sf::Mouse::Left).isHolding)
	{
		MapBlockState blockState = _ghostBlock->getState();
		_map->addBlock(blockState);
		_map->save();
	}
	if (WindowInput::getMouseButtonState(sf::Mouse::Right).isHolding)
	{
		_map->removeBlockAt(_ghostBlock->getState().position);
		_map->save();
	}
}

MapEditorEngine::MapEditorEngine(const std::string& mapName, const sf::Vector2u& size, const std::string& windowTitle) : Engine(size, windowTitle)
{
	_map = std::make_unique<Map>(mapName);
	_ghostBlock = std::make_unique<GhostMapBlock>();
}

MapEditorEngine::~MapEditorEngine() { }

void MapEditorEngine::onStart()
{
	_map->load();
}

void MapEditorEngine::onUpdate()
{
	processGhostBlockTypeSelecting();
	processGhostBlockPosition();
	processGhostBlockPlacing();
}

void MapEditorEngine::onRender(std::function<void(sf::Drawable& drawable)> render)
{
	render(*_map);
	render(*_ghostBlock);
}

void MapEditorEngine::onEvent(sf::Event& event)
{
}
