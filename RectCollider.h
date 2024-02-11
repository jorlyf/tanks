#pragma once
#include <SFML/Graphics.hpp>

class RectCollider : public sf::Drawable
{
	sf::Vertex _vertices[5];
	sf::Sprite* _sprite;

protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
	RectCollider();
	RectCollider(sf::Sprite* sprite);
	void calculateVertices();
	bool intersects(RectCollider& other);
};
