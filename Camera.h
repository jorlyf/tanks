#pragma once
#include <SFML/System/Vector2.hpp>

class Window;

class Camera
{
	Window* _window = nullptr;

	sf::Vector2f _targetPosition;
	sf::Vector2f _cameraPosition;

	static sf::Vector2f lerp(const sf::Vector2f& a, const sf::Vector2f& b, const float k);

public:
	Camera();
	Camera(const sf::Vector2f& position);

	void update();
	void setWindow(Window* window);
	void setTargetPosition(const sf::Vector2f& position);
};
