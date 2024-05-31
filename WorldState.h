#pragma once
#include <guiddef.h>
#include <SFML/System/Vector2.hpp>
#include "Team.h"

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
	Team team;
	sf::Vector2f cursorPosition;
	TankState tank;
	bool isTankAlive;
};

struct WorldState
{
	unsigned char playerCount;
	PlayerState players[8];
	unsigned char bulletCount;
	BulletState bullets[64];
};