#ifndef CUBED_INPUT_MANAGER_H
#define CUBED_INPUT_MANAGER_H

#include <sdl2/include/SDL.h>
#include <array>
#include <vector>
#include <functional>

class InputManager
{
public:
	InputManager();
	InputManager(const InputManager&) = delete;

	enum Key
	{
		KEY_A = SDL_SCANCODE_A,
		KEY_B = SDL_SCANCODE_B,
		KEY_C = SDL_SCANCODE_C,
		KEY_D = SDL_SCANCODE_D,
		KEY_E = SDL_SCANCODE_E,
		KEY_F = SDL_SCANCODE_F,
		KEY_G = SDL_SCANCODE_G,
		KEY_H = SDL_SCANCODE_H,
		KEY_I = SDL_SCANCODE_I,
		KEY_J = SDL_SCANCODE_J,
		KEY_K = SDL_SCANCODE_K,
		KEY_L = SDL_SCANCODE_L,
		KEY_M = SDL_SCANCODE_M,
		KEY_N = SDL_SCANCODE_N,
		KEY_O = SDL_SCANCODE_O,
		KEY_P = SDL_SCANCODE_P,
		KEY_Q = SDL_SCANCODE_Q,
		KEY_R = SDL_SCANCODE_R,
		KEY_S = SDL_SCANCODE_S,
		KEY_T = SDL_SCANCODE_T,
		KEY_U = SDL_SCANCODE_U,
		KEY_V = SDL_SCANCODE_V,
		KEY_W = SDL_SCANCODE_W,
		KEY_X = SDL_SCANCODE_X,
		KEY_Y = SDL_SCANCODE_Y,
		KEY_Z = SDL_SCANCODE_Z,
		KEY_ESC = SDL_SCANCODE_ESCAPE
	};

	enum MouseButton
	{
		MOUSE_LEFT = SDL_BUTTON_LEFT,
		MOUSE_MIDDLE = SDL_BUTTON_MIDDLE,
		MOUSE_RIGHT = SDL_BUTTON_RIGHT
	};

	enum
	{
		MOUSE_LEFT_INDEX = 0,
		MOUSE_MIDDLE_INDEX,
		MOUSE_RIGHT_INDEX
	};

	void handle_key_down(Key k);
	void handle_key_up(Key k);
	void handle_mouse_down(MouseButton mb);
	void handle_mouse_up(MouseButton mb);

	bool is_key_down(Key k) { return m_key_states[k]; }

	void add_key_down_handler(Key k, std::function<void()> handler) { m_key_down_handlers.emplace_back(k, handler); }
	void add_key_up_handler(Key k, std::function<void()> handler) { m_key_up_handlers.emplace_back(k, handler); }
	void add_mouse_down_handler(MouseButton mb, std::function<void()> handler) { m_mouse_down_handlers.emplace_back(mb, handler); }
	void add_mouse_up_handler(MouseButton mb, std::function<void()> handler) { m_mouse_up_handlers.emplace_back(mb, handler); }

	void request_quit() { m_quit = true; }
	auto is_quit_requested() const { return m_quit; }

private:
	bool m_quit;
	std::array<bool, SDL_NUM_SCANCODES> m_key_states;
	std::array<bool, 3> m_mouse_button_states;
	std::vector<std::pair<Key, std::function<void()>>> m_key_down_handlers;
	std::vector<std::pair<Key, std::function<void()>>> m_key_up_handlers;
	std::vector<std::pair<MouseButton, std::function<void()>>> m_mouse_down_handlers;
	std::vector<std::pair<MouseButton, std::function<void()>>> m_mouse_up_handlers;
};

#endif