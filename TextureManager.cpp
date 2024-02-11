#include "TextureManager.h"

sf::Texture TextureManager::tank;
sf::Texture TextureManager::tankCannon;
sf::Texture TextureManager::bullet;
std::string TextureManager::_texturesFolderPath = "Resources/Textures/";

void TextureManager::load()
{
	tank.loadFromFile(_texturesFolderPath + "tank-texture.png");
	tankCannon.loadFromFile(_texturesFolderPath + "tank-cannon-texture.png");
	bullet.loadFromFile(_texturesFolderPath + "fire-bullet.png");

	tank.setSmooth(true);
	tankCannon.setSmooth(true);
	bullet.setSmooth(true);
}