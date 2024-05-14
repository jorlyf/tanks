#include "Window.h"

Window::Window(const sf::Vector2u& size, const std::string& title)
{
	_window = std::make_unique<sf::RenderWindow>(sf::VideoMode{ size.x, size.y }, title);
	_window->setVerticalSyncEnabled(true);

	resize(size);
}

void Window::clear(sf::Color color)
{
	_window->clear(color);
}

void Window::draw(sf::Drawable& drawable) const
{
	_window->draw(drawable);
}

void Window::display() const
{
	_window->display();
}

void Window::close()
{
	_window->close();
}

void Window::resize(const sf::Vector2u& size) // TODO: fix incorrect resizing
{
	sf::View view{ {0.f, 0.f}, {(float)size.x, (float)size.y} };
	_window->setView(view);
}

bool Window::isOpen() const
{
	return _window->isOpen();
}

bool Window::pollEvent(sf::Event& event)
{
	return _window->pollEvent(event);
}

sf::Vector2i Window::getRelativeMousePosition() const
{
	return sf::Mouse::getPosition(*_window);
}

sf::Vector2f Window::getWorldMousePosition() const
{
	sf::Vector2i relativePosition = getRelativeMousePosition();
	sf::Vector2f worldPosition = _window->mapPixelToCoords(relativePosition);
	return worldPosition;
}

void Window::setView(const sf::View& view)
{
	_window->setView(view);
}
