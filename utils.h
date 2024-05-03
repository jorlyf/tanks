#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <functional>
#include <string>

void logVector2f(const sf::Vector2f& vector);

void drawSpriteGlobalBounds(const sf::Sprite& sprite, sf::RenderTarget& target, sf::RenderStates states);

float degreesToRadians(const float degrees);

float radiansToDegrees(const float radians);

sf::Vector2f angleToDirection(const float angle);

float directionToAngle(const sf::Vector2f& vector);

float normalizeAngle(float angle);

bool isFileExist(const std::string& filePath);

struct Vector2iComparator
{
	bool operator()(const sf::Vector2i& a, const sf::Vector2i& b) const;
};

bool isPolygonsIntersecting(const sf::Vertex* a, const int aSize, const sf::Vertex* b, const int bSize);

template<typename T>
size_t getAddress(std::function<void(T)> f)
{
	typedef void (fnType)(T);
	fnType** fnPointer = f.template target<fnType*>();
	return (size_t)*fnPointer;
}
