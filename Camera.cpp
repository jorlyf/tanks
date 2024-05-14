#include "Camera.h"
#include "DeltaTime.h"
#include "Window.h"

sf::Vector2f Camera::lerp(const sf::Vector2f& a, const sf::Vector2f& b, const float k)
{
	const sf::Vector2f d = b - a;
	return sf::Vector2f{ a.x + d.x * k, a.y + d.y * k };
}

Camera::Camera() {}

Camera::Camera(const sf::Vector2f& position)
{
	_cameraPosition = position;
	_targetPosition = position;
}

void Camera::update()
{
	_cameraPosition = lerp(_cameraPosition, _targetPosition, 4 * DeltaTime::getDt());

	const float width = 1200, height = 800;
	sf::View view(_cameraPosition, { width, height });
	_window->setView(view);
}

void Camera::setWindow(Window* window)
{
	_window = window;
}

void Camera::setTargetPosition(const sf::Vector2f& position)
{
	_targetPosition = position;
}
