#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

void logVector2f(const sf::Vector2f& vector);

void drawSpriteGlobalBounds(const sf::Sprite& sprite, sf::RenderTarget& target, sf::RenderStates states);

float degreesToRadians(const float degrees);

float radiansToDegrees(const float radians);

sf::Vector2f angleToDirection(const float angle);

float directionToAngle(const sf::Vector2f& vector);

float normalizeAngle(float angle);