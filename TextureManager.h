#pragma once
#include <memory>
#include <string>
#include <SFML/Graphics/Texture.hpp>

static class TextureManager
{
	static std::string _textureFolderPath;

public:
	static sf::Texture tank;
	static sf::Texture tankCannon;
	static sf::Texture bullet;

	static sf::Texture metalBlock;
	static sf::Texture redBaseBlock;
	static sf::Texture blueBaseBlock;

	static void load();
};
