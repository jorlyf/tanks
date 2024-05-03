#include "TextureManager.h"
#include "FolderPathManager.h"

sf::Texture TextureManager::tank;
sf::Texture TextureManager::tankCannon;
sf::Texture TextureManager::bullet;
sf::Texture TextureManager::metalBlock;
std::string TextureManager::_textureFolderPath = FolderPathManager::textureFolderPath;

void TextureManager::load()
{
	tank.loadFromFile(_textureFolderPath + "tank-texture.png");
	tankCannon.loadFromFile(_textureFolderPath + "tank-cannon-texture.png");
	bullet.loadFromFile(_textureFolderPath + "fire-bullet.png");

	tank.setSmooth(true);
	tankCannon.setSmooth(true);
	bullet.setSmooth(true);

	metalBlock.loadFromFile(_textureFolderPath + "metal-block.png");
}