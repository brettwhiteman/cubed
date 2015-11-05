#ifndef CUBED_WINDOW_H
#define CUBED_WINDOW_H

#include <string>
#include <sdl2/include/SDL.h>
#include <functional>
#include <vector>
#include <utility>

class input_manager;

class window
{
public:
	window(const std::string& title, int width, int height, input_manager& input);
	~window();
	window(const window&) = delete;

	void update();
	void swap_buffers();
	void add_resize_handler(std::function<void(const std::pair<int, int>&)> handler) { m_window_resize_handlers.emplace_back(std::move(handler)); }

	const std::pair<int, int>& get_window_size() const { return m_window_size; }

private:
	SDL_Window* m_window;
	SDL_GLContext m_context;
	input_manager& m_input;
	std::pair<int, int> m_window_size;
	std::vector<std::function<void(const std::pair<int, int>&)>> m_window_resize_handlers;
};

#include "cubed_exception.h"

class window_exception : public cubed_exception
{
public:
	window_exception(std::string message) : cubed_exception(std::move(message)) { }
};

#endif