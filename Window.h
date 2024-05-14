#pragma once
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <SFML/Graphics.hpp>

class Window
{
	std::unique_ptr<sf::RenderWindow> _window;

public:
	Window(const sf::Vector2u& size, const std::string& title);
	void clear(sf::Color color = sf::Color(0, 0, 0));
	void draw(sf::Drawable& drawable) const;
	void display() const;
	void close();
	void resize(const sf::Vector2u& size);
	bool isOpen() const;
	bool pollEvent(sf::Event& event);
	sf::Vector2i getRelativeMousePosition() const;
	sf::Vector2f getWorldMousePosition() const;
	void setView(const sf::View& view);
};
