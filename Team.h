#pragma once
#include <list>
#include <memory>

class Player;

enum Team : unsigned char
{
	red,
	blue
};

Team getLowestPlayerTeam(const std::list<std::unique_ptr<Player>>& players);
