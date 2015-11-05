#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <functional>
#include <vector>
#include <array>
#include <sdl2/include/SDL.h>

#define STATE_NORMAL 1
#define STATE_INVENTORY 2

struct Handler
{
	Handler(SDL_Scancode code, std::function<void()> func) : code(code), func(func) {}

	SDL_Scancode code;
	std::function<void()> func;
};

class InputManager
{
public:
	InputManager();

	void update();
	std::pair<int, int> getMouseOffset();
	std::pair<int, int> getMousePosition();
	bool keyDown(SDL_Keycode keyCode) { return m_keyStates[keyCode]; }
	bool mouseLeft() { return static_cast<bool>(m_mouseState & SDL_BUTTON(SDL_BUTTON_LEFT)); }
	bool mouseRight() { return static_cast<bool>(m_mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT)); }
	void addKeyPressHandler(SDL_Scancode keyCode, std::function<void()> func);
	void addKeyReleaseHandler(SDL_Scancode keyCode, std::function<void()> func);
	void addLeftClickHandler(std::function<void()> func);
	void addRightClickHandler(std::function<void()> func);
	void handleKeyDownEvent(SDL_Scancode key);
	void handleKeyUpEvent(SDL_Scancode key);
	void centreMouse();
	void handleLeftClick();
	void handleRightClick();

	static InputManager& instance();

	int m_state;

private:
	std::vector<Handler> m_keyPressHandlers;
	std::vector<Handler> m_keyReleaseHandlers;
	std::vector<std::function<void()>> m_leftClickHandlers;
	std::vector<std::function<void()>> m_rightClickHandlers;
	std::array<bool, SDL_NUM_SCANCODES> m_keyStates;
	int m_mouseX;
	int m_mouseY;
	Uint32 m_mouseState;
};

#endif