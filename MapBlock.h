#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include <functional>
#include "RectCollider.h"
#include "MapState.h"

class MapBlock : public sf::Drawable
{
	MapBlockType _type;
	sf::Vector2i _position;
	float _health;

	const sf::Texture* _texture = nullptr;
	sf::Sprite _sprite;

	RectCollider _collider;

	std::list<std::function<void(MapBlock* mapBlock)>> _onBlockDestroy;

	void notifyMapBlockDestroyListeners();

	void initSprite();
	sf::Vector2f getWorldPosition() const;
	bool isUnbreakable() const;

protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
	MapBlock(const MapBlockState& state);
	void update();
	MapBlockType getType() const;
	sf::Vector2i getPosition() const;
	float getHealth() const;
	RectCollider getCollider() const;
	void damage(const float damage);

	void addOnMapBlockDestroyListener(std::function<void(MapBlock* mapBlock)> listener);

	static int SIZE;
	static sf::Vector2f getWorldPosition(const sf::Vector2i position);
	static sf::Texture* getTexture(const MapBlockType type);
};

