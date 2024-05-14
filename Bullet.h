#pragma once
#include <guiddef.h>
#include <list>
#include <functional>
#include <SFML/Graphics.hpp>
#include "RectCollider.h"
#include "Event.h"

class World;
class MapBlock;
class Tank;

class Bullet : public sf::Drawable
{
	World* _world;

	unsigned int _id;
	GUID _playerGuid;
	sf::Vector2f _position;
	sf::Vector2f _direction;
	float _speed;
	float _damage = 80.f;

	sf::Sprite _sprite;

	RectCollider _collider;

	void initSprite();
	void move();
	bool isCollideMap(const sf::Vector2f& position, const float rotation, MapBlock** mapBlockPtr);
	bool isCollideTank(const sf::Vector2f& position, const float rotation, Tank** tankPtr);

	Event<MapBlock*> _onMapBlockHit;
	Event<Tank*> _onTankHit;

protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
	Bullet(const unsigned int id, const GUID& playerGuid, World* world, const sf::Vector2f& position, const sf::Vector2f& direction, const float maxSpeed);
	void update();
	unsigned int getId() const;
	GUID getPlayerGuid() const;
	sf::Vector2f getPosition() const;
	void setPosition(const sf::Vector2f& position);
	sf::Vector2f getDirection() const;
	void setDirection(const sf::Vector2f& direction);
	float getSpeed() const;
	float getDamage() const;
	void addOnMapBlockHitListener(const EventHandler<MapBlock*>& handler);
	void addOnTankHitListener(const EventHandler<Tank*>& handler);
};
