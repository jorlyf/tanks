#pragma once
#include <SFML/Graphics.hpp>

class Map : public sf::Drawable
{
	std::string _name;

	std::unique_ptr<sf::Texture> _backgroundTexture = nullptr;
	sf::Sprite _backgroundSprite;

	bool _loaded;

protected:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
	Map(const std::string& name);
	void load();
	void update();
};
