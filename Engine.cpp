#include "Engine.h"

void Engine::start()
{
	onStart();
}

void Engine::pollEvents()
{
	sf::Event event;
	while (_window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed) _window->close();
		else if (event.type == sf::Event::Resized) _window->resize({ event.size.width, event.size.height });
		else onEvent(event);
	}
}

Engine::Engine(const sf::Vector2u& size, const std::string& windowTitle)
{
	_window = std::make_unique<Window>(size, windowTitle);
}

Engine::~Engine() { }

void Engine::run()
{
	start();

	while (_window->isOpen())
	{
		pollEvents();

		update();
		render();
	}
}

void Engine::update()
{
	DeltaTime::update();
	onUpdate();
	WindowInput::setWorldMousePosition(_window->getWorldMousePosition());
	WindowInput::update();
}

void Engine::render()
{
	_window->clear();
	onRender([this](sf::Drawable& drawable) { _window->draw(drawable); });
	_window->display();
}
