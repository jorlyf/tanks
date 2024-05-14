#include "FontManager.h"
#include "FolderPathManager.h"

sf::Font FontManager::regular;
std::string FontManager::_fontFolderPath = FolderPathManager::fontFolderPath;

void FontManager::load()
{
	regular.loadFromFile(_fontFolderPath + "Roboto\\Roboto-Regular.ttf");
}