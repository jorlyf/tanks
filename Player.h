#pragma once
#include <guiddef.h>
#include <SFML/Graphics.hpp>
#include "WindowInput.h"

class World;
class Tank;

class Player : public sf::Drawable
{
public:
	struct PlayerInput
	{
		InputState w, a, s, d, lmb;
		sf::Vector2f worldMousePosition;
	};

private:
	World* _world = nullptr;
	GUID _guid;
	std::unique_ptr<Tank> _tank = nullptr;
	PlayerInput _input;

	void processMovingInput();
	void processFiringInput();
	void setCannonTargetRotation();

protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
	Player(const GUID& guid, World* world);
	void update();
	GUID getGuid() const;
	Tank* getTank();
	Tank* spawnTank(const sf::Vector2f& position, const float rotation);
	void destroyTank();
	void setInput(const PlayerInput& playerInput);
};
