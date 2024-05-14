#pragma once
#include <memory>
#include <string>
#include <SFML/Graphics/Font.hpp>

static class FontManager
{
	static std::string _fontFolderPath;

public:
	static sf::Font regular;

	static void load();
};
