#include <cmath>
#include <iostream>
#include <SFML/Graphics/Vertex.hpp>
#include "utils.h"

void logVector2f(const sf::Vector2f& vector)
{
	std::cout << vector.x << " " << vector.y << std::endl;
}

void drawSpriteGlobalBounds(const sf::Sprite& sprite, sf::RenderTarget& target, sf::RenderStates states)
{
	const sf::FloatRect rect = sprite.getGlobalBounds();
	sf::Vertex vertices[5];
	for (int i = 0; i < 5; i++)
	{
		vertices[i].color = sf::Color::Red;
	}

	const sf::Vector2f leftTopPoint = rect.getPosition();

	vertices[0].position = leftTopPoint;
	vertices[1].position = leftTopPoint + sf::Vector2f{ rect.width, 0 };
	vertices[2].position = leftTopPoint + sf::Vector2f{ rect.width, rect.height };
	vertices[3].position = leftTopPoint + sf::Vector2f{ 0, rect.height };
	vertices[4].position = leftTopPoint;

	target.draw(vertices, 5, sf::PrimitiveType::LinesStrip, states);
}

float degreesToRadians(const float degrees)
{
	return degrees * 3.14f / 180.f;
}

float radiansToDegrees(const float radians)
{
	return radians / 3.14f * 180.f;
}

sf::Vector2f angleToDirection(const float angle)
{
	const float radianAngle = degreesToRadians(angle);
	return sf::Vector2f{ std::cosf(radianAngle), std::sinf(radianAngle) };
}

float directionToAngle(const sf::Vector2f& direction)
{
	return radiansToDegrees(std::atan2(direction.y, direction.x));
}

float normalizeAngle(float angle)
{
	while (angle < 0)
		angle += 360.f;
	return std::fmodf(angle, 360.f);
}