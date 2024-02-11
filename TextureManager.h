#pragma once
#include <memory>
#include <string>
#include <SFML/Graphics/Texture.hpp>

static class TextureManager
{
private:
	static std::string _texturesFolderPath;

public:
	static sf::Texture tank;
	static sf::Texture tankCannon;
	static sf::Texture bullet;

	static void load();
};
