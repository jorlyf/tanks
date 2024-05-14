#include "RectCollider.h"
#include "utils.h"

void RectCollider::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(_vertices, 5, sf::PrimitiveType::LinesStrip, states);
	target.draw(_vertices, 2, sf::PrimitiveType::LinesStrip, states);
}

RectCollider::RectCollider()
{

}

RectCollider::RectCollider(sf::Sprite* sprite)
{
	_sprite = sprite;
	for (int i = 0; i < 5; i++)
	{
		_vertices[i].color = sf::Color::Red;
	}
	calculateVertices();
}

void RectCollider::calculateVertices()
{
	if (_sprite == nullptr) return;
	const sf::Vector2f scale = _sprite->getScale();
	const sf::Vector2f origin = sf::Vector2f{ _sprite->getOrigin().x * scale.x, _sprite->getOrigin().y * scale.y };
	const sf::FloatRect localBounds = _sprite->getLocalBounds();

	const float width = localBounds.width * scale.x;
	const float height = localBounds.height * scale.y;

	const sf::Vector2f localLeftTopPoint = sf::Vector2f{ localBounds.getPosition().x * scale.x, localBounds.getPosition().y * scale.y } - origin;

	_vertices[0].position = localLeftTopPoint;
	_vertices[1].position = { localLeftTopPoint.x + width, localLeftTopPoint.y };
	_vertices[2].position = { localLeftTopPoint.x + width, localLeftTopPoint.y + height };
	_vertices[3].position = { localLeftTopPoint.x, localLeftTopPoint.y + height };
	_vertices[4].position = localLeftTopPoint;

	const sf::Vector2f position = _sprite->getPosition();

	const float rotation = degreesToRadians(_sprite->getRotation());
	const float c = std::cos(rotation);
	const float s = std::sin(rotation);

	for (int i = 0; i < 5; i++)
	{
		const float x = _vertices[i].position.x * c + _vertices[i].position.y * s;
		const float y = -_vertices[i].position.y * c + _vertices[i].position.x * s;
		_vertices[i].position = { x + position.x, y + position.y };
	}
}

bool RectCollider::intersects(RectCollider& other)
{
	calculateVertices();
	other.calculateVertices();

	return isPolygonsIntersecting(_vertices, 5, other._vertices, 5);
}
