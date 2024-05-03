#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include <functional>
#include "RectCollider.h"

class World;
class Player;
class TankCannon;
class MapBlock;

class Tank : public sf::Drawable
{
	World* _world = nullptr;
	Player* _player = nullptr;

	float _hp = 100.f;
	float _mass = 1000.f;
	float _speed = 100.f;
	float _rotationSpeed = 80.f;

	sf::Vector2f _position{ 0.f, 0.f };
	float _rotation = 0.f;

	std::unique_ptr<TankCannon> _cannon = nullptr;

	sf::Sprite _sprite;

	RectCollider _collider;

	std::list<std::function<void(Tank* tank)>> _onTankDestroy;

	void initSprite();
	bool isCollideMap(const sf::Vector2f& position, const float rotation);

protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
	Tank(World* world, Player* player, const sf::Vector2f position, const float rotation);
	void update();
	void moveForward();
	void moveBackward();
	void rotateLeft();
	void rotateRight();
	float getRotation() const;
	void setRotation(const float rotation);
	sf::Vector2f getPosition() const;
	void setPosition(const sf::Vector2f& position);
	float getHp() const;
	void setHp(const float hp);
	float damage(const float damage);
	Player* getPlayer() const;
	TankCannon* getCannon() const;
	RectCollider getCollider() const;
};
