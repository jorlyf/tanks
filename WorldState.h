#pragma once
#include <guiddef.h>
#include <SFML/System/Vector2.hpp>

struct BulletState
{
	unsigned int id;
	GUID playerGuid;
	sf::Vector2f position;
	sf::Vector2f direction;
	float speed;
};

struct TankCannonState
{
	float rotation;
	float targetRotation;
};

struct TankState
{
	float hp;
	sf::Vector2f position;
	float rotation;
	TankCannonState cannon;
};

struct PlayerState
{
	GUID guid;
	sf::Vector2f cursorPosition;
	TankState tank;
};

struct WorldState
{
	unsigned char playerCount;
	PlayerState players[4];
	unsigned char bulletCount;
	BulletState bullets[16];
};