#pragma once
#include <guiddef.h>
#include <SFML/Graphics.hpp>

class Bullet : public sf::Drawable
{
	unsigned int _id;
	GUID _playerGuid;
	sf::Vector2f _position;
	sf::Vector2f _direction;
	float _speed;

	sf::Sprite _sprite;

	void initSprite();
	void move();

protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
	Bullet(const unsigned int id, const GUID& playerGuid, const sf::Vector2f& position, const sf::Vector2f& direction, const float maxSpeed);
	void update();
	unsigned int getId() const;
	GUID getPlayerGuid() const;
	sf::Vector2f getPosition() const;
	void setPosition(const sf::Vector2f& position);
	sf::Vector2f getDirection() const;
	void setDirection(const sf::Vector2f& direction);
	float getSpeed() const;
	float getDamage() const;
};
