#pragma once
#include <SFML/Graphics.hpp>
#include "RectCollider.h"

class World;
class Player;
class TankCannon;

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

	void initSprite();

protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
	Tank(World* world, Player* player);
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
	Player* getPlayer() const;
	TankCannon* getCannon() const;
};
