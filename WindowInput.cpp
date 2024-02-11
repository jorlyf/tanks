#include "WindowInput.h"

std::map<sf::Keyboard::Key, InputState> WindowInput::_keyboardKeys;
std::map<sf::Mouse::Button, InputState> WindowInput::_mouseButtons;
sf::Vector2f WindowInput::_worldMousePosition;

void WindowInput::update()
{
	for (auto& item : _keyboardKeys)
	{
		InputState& state = item.second;
		state.isPressed = false;
	}
	for (auto& item : _mouseButtons)
	{
		InputState& state = item.second;
		state.isPressed = false;
	}
}

void WindowInput::handleKeyboardKeyPressed(sf::Keyboard::Key keyboardKey)
{
	InputState& state = _keyboardKeys[keyboardKey];
	state.isHolding = true;
	state.isPressed = true;
}

void WindowInput::handleMouseButtonPressed(sf::Mouse::Button mouseButton)
{
	InputState& state = _mouseButtons[mouseButton];
	state.isHolding = true;
	state.isPressed = true;
}

void WindowInput::handleKeyboardKeyReleased(sf::Keyboard::Key keyboardKey)
{
	InputState& state = _keyboardKeys[keyboardKey];
	state.isHolding = false;
	state.isPressed = false;
}

void WindowInput::handleMouseButtonReleased(sf::Mouse::Button mouseButton)
{
	InputState& state = _mouseButtons[mouseButton];
	state.isHolding = false;
	state.isPressed = false;
}

InputState WindowInput::getKeyboardKeyState(sf::Keyboard::Key keyboardKey)
{
	return _keyboardKeys[keyboardKey];
}

InputState WindowInput::getMouseButtonState(sf::Mouse::Button mouseButton)
{
	return _mouseButtons[mouseButton];
}

void WindowInput::setWorldMousePosition(const sf::Vector2f& worldMousePosition)
{
	_worldMousePosition = worldMousePosition;
}

sf::Vector2f WindowInput::getWorldMousePosition()
{
	return _worldMousePosition;
}
