#include "input_manager.h"

InputManager::InputManager() :
	m_quit(false)
{
	m_key_states.fill(false);
	m_mouse_button_states.fill(false);
}

void InputManager::handle_key_down(Key key)
{
	m_key_states[key] = true;

	for(auto& handler : m_key_down_handlers)
	{
		if(handler.first == key)
		{
			handler.second();
		}
	}
}

void InputManager::handle_key_up(Key key)
{
	m_key_states[key] = false;

	for(auto& handler : m_key_up_handlers)
	{
		if(handler.first == key)
		{
			handler.second();
		}
	}
}

void InputManager::handle_mouse_down(MouseButton mouse_button)
{
	switch(mouse_button)
	{
		case MOUSE_LEFT:
			m_mouse_button_states[MOUSE_LEFT_INDEX] = true;
			break;

		case MOUSE_RIGHT:
			m_mouse_button_states[MOUSE_RIGHT_INDEX] = true;
			break;

		case MOUSE_MIDDLE:
			m_mouse_button_states[MOUSE_MIDDLE_INDEX] = true;
			break;
	}

	for(auto& handler : m_mouse_down_handlers)
	{
		if(handler.first == mouse_button)
		{
			handler.second();
		}
	}
}

void InputManager::handle_mouse_up(MouseButton mouse_button)
{
	switch (mouse_button)
	{
	case MOUSE_LEFT:
		m_mouse_button_states[MOUSE_LEFT_INDEX] = false;
		break;

	case MOUSE_RIGHT:
		m_mouse_button_states[MOUSE_RIGHT_INDEX] = false;
		break;

	case MOUSE_MIDDLE:
		m_mouse_button_states[MOUSE_MIDDLE_INDEX] = false;
		break;
	}

	for(auto& handler : m_mouse_up_handlers)
	{
		if(handler.first == mouse_button)
		{
			handler.second();
		}
	}
}

void InputManager::set_mouse_offset(std::pair<int, int> mouse_offset)
{
	for (auto& handler : m_mouse_move_handlers)
	{
		handler(mouse_offset);
	}
}