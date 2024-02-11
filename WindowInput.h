#pragma once
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <map>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

struct InputState
{
	bool isPressed;
	bool isHolding;
};

static class WindowInput
{
	static std::map<sf::Keyboard::Key, InputState> _keyboardKeys;
	static std::map<sf::Mouse::Button, InputState> _mouseButtons;
	static sf::Vector2f _worldMousePosition;

public:
	static void update();
	static void handleKeyboardKeyPressed(sf::Keyboard::Key keyboardKey);
	static void handleMouseButtonPressed(sf::Mouse::Button mouseButton);
	static void handleKeyboardKeyReleased(sf::Keyboard::Key keyboardKey);
	static void handleMouseButtonReleased(sf::Mouse::Button mouseButton);
	static InputState getKeyboardKeyState(sf::Keyboard::Key keyboardKey);
	static InputState getMouseButtonState(sf::Mouse::Button mouseButton);
	static void setWorldMousePosition(const sf::Vector2f& worldMousePosition);
	static sf::Vector2f getWorldMousePosition();
};
