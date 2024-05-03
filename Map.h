#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include "MapState.h"
#include "utils.h"

class MapBlock;

class Map : public sf::Drawable
{
	std::string _name;

	std::unique_ptr<sf::Texture> _backgroundTexture = nullptr;
	sf::Sprite _backgroundSprite;

	std::map<sf::Vector2i, std::unique_ptr<MapBlock>, Vector2iComparator> _blocks;
	std::list<sf::Vector2i> _blockPositionsToDelete;

	bool _loaded = false;

protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
	Map(const std::string& name);
	void load();
	void save() const;
	void update();
	std::map<sf::Vector2i, std::unique_ptr<MapBlock>, Vector2iComparator>* getBlocks();
	void addBlock(const MapBlockState& blockState);
	void removeBlockAt(const sf::Vector2i& position);
	MapState getMapState() const;
	void setMapState(const MapState& state);
};
