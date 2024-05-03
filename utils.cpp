#include <sys/stat.h>
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

bool isFileExist(const std::string& filePath)
{
	struct stat buffer;
	return stat(filePath.c_str(), &buffer) == 0;
}

bool Vector2iComparator::operator()(const sf::Vector2i& a, const sf::Vector2i& b) const
{
	if (a.x < b.x) return true;
	if (a.x == b.x && a.y < b.y) return true;
	return false;
}

bool isPolygonsIntersecting(const sf::Vertex* a, const int aSize, const sf::Vertex* b, const int bSize)
{
	for (int k = 0; k < 2; k++)
	{
		const sf::Vertex* polygon = k == 0 ? a : b;
		const int size = k == 0 ? aSize : bSize;
		for (int i1 = 0; i1 < size; i1++)
		{
			int i2 = (i1 + 1) % size;
			const sf::Vertex& p1 = polygon[i1];
			const sf::Vertex& p2 = polygon[i2];

			sf::Vector2f normal(p2.position.y - p1.position.y, p1.position.x - p2.position.x);

			float minA = 1000000.0f, maxA = -1000000.0f;
			for (int pi = 0; pi < aSize; pi++)
			{
				const sf::Vertex& p = a[pi];
				const float projected = normal.x * p.position.x + normal.y * p.position.y;
				if (projected < minA)
					minA = projected;
				if (projected > maxA)
					maxA = projected;
			}

			float minB = 1000000.0f, maxB = -1000000.0f;
			for (int pi = 0; pi < bSize; pi++)
			{
				const sf::Vertex& p = b[pi];
				const float projected = normal.x * p.position.x + normal.y * p.position.y;
				if (projected < minB)
					minB = projected;
				if (projected > maxB)
					maxB = projected;
			}

			if (maxA < minB || maxB < minA)
				return false;
		}
	}

	return true;
}

//template <typename T>
//size_t getAddress(const std::function<void(T&)>& function)
//{
//	typedef void (fnType)(T&);
//	fnType** fnPointer = (*function).target<fnType*>();
//	return (size_t)*fnPointer;
//}
