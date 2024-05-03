#pragma once
#include "Engine.h"

class Map;
class GhostMapBlock;

class MapEditorEngine : public Engine
{
	std::unique_ptr<Map> _map = nullptr;

	std::unique_ptr<GhostMapBlock> _ghostBlock = nullptr;

	void processGhostBlockTypeSelecting();
	void processGhostBlockPosition();
	void processGhostBlockPlacing();

public:
	MapEditorEngine(const std::string& mapName, const sf::Vector2u& size, const std::string& windowTitle = "Map editor");
	virtual ~MapEditorEngine();
	virtual void onStart() override;
	virtual void onUpdate() override;
	virtual void onRender(std::function<void(sf::Drawable& drawable)> render) override;
	virtual void onEvent(sf::Event& event) override;
};
