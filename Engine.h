#pragma once
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <functional>
#include "Window.h"
#include "DeltaTime.h"
#include "WindowInput.h"

class Engine
{
	void start();
	void pollEvents();

protected:
	std::unique_ptr<Window> _window;

public:
	Engine(const sf::Vector2u& size, const std::string& windowTitle = "Unnamed window");
	virtual ~Engine();
	void run();
	void update();
	void render();
	virtual void onStart() = 0;
	virtual void onUpdate() = 0;
	virtual void onRender(std::function<void(sf::Drawable& drawable)> render) = 0;
	virtual void onEvent(sf::Event& event) = 0;
};
