#include "window.h"
#include "input_manager.h"

window::window(const std::string& title, int width, int height, input_manager& input)
	: m_input(input)
{
	if(SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		throw window_exception("SDL_Init() failed");
	}

	m_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

	if(!m_window)
	{
		throw window_exception("SDL_CreateWindow() failed");
	}

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

	m_context = SDL_GL_CreateContext(m_window);

	if(!m_context)
	{
		throw window_exception("SDL_GL_CreateContext() failed");
	}

	m_window_size = std::make_pair(width, height);
}

window::~window()
{
	SDL_GL_DeleteContext(m_context);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}

void window::update()
{
	SDL_Event e;

	while(SDL_PollEvent(&e))
	{
		switch(e.type)
		{
			case SDL_QUIT:
				m_input.request_quit();
				break;

			case SDL_KEYDOWN:
				m_input.handle_key_down(static_cast<input_manager::key>(e.key.keysym.scancode));
				break;

			case SDL_KEYUP:
				m_input.handle_key_up(static_cast<input_manager::key>(e.key.keysym.scancode));
				break;

			case SDL_MOUSEBUTTONDOWN:
				m_input.handle_mouse_down(static_cast<input_manager::mouse_button>(e.button.button));
				break;

			case SDL_MOUSEBUTTONUP:
				m_input.handle_mouse_up(static_cast<input_manager::mouse_button>(e.button.button));
				break;

			case SDL_WINDOWEVENT:
				switch (e.window.event)
				{
					case SDL_WINDOWEVENT_SIZE_CHANGED:
						m_window_size = std::make_pair(e.window.data1, e.window.data2);
						
						for (auto& handler : m_window_resize_handlers)
						{
							handler(m_window_size);
						}

						break;
				}

				break;
		}
	}
}

void window::swap_buffers()
{
	SDL_GL_SwapWindow(m_window);
}