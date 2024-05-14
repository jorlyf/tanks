#include <list>
#include <map>
#include "Team.h"
#include "Player.h"

Team getLowestPlayerTeam(const std::list<std::unique_ptr<Player>>& players)
{
	std::map<Team, unsigned char> count{ {Team::red, 0}, {Team::blue, 0} };
	for (auto& player : players)
	{
		count[player->getTeam()]++;
	}

	Team team(Team::red);
	unsigned char min = 255;
	for (auto it = count.cbegin(); it != count.cend(); it++)
	{
		if (it->second < min)
		{
			team = it->first;
			min = it->second;
		}
	}

	return team;
}
