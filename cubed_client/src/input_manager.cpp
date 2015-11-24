#include "input_manager.h"

InputManager::InputManager() :
	m_quit(false),
	m_mouse_offset(0, 0)
{
	m_key_states.fill(false);
	m_mouse_button_states.fill(false);
}

void InputManager::handle_key_down(Key k)
{
	m_key_states[k] = true;

	for(auto& h : m_key_down_handlers)
	{
		if(h.first == k)
		{
			h.second();
		}
	}
}

void InputManager::handle_key_up(Key k)
{
	m_key_states[k] = false;

	for(auto& h : m_key_up_handlers)
	{
		if(h.first == k)
		{
			h.second();
		}
	}
}

void InputManager::handle_mouse_down(MouseButton mb)
{
	switch(mb)
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

	for(auto& h : m_mouse_down_handlers)
	{
		if(h.first == mb)
		{
			h.second();
		}
	}
}

void InputManager::handle_mouse_up(MouseButton mb)
{
	switch (mb)
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

	for(auto& h : m_mouse_up_handlers)
	{
		if(h.first == mb)
		{
			h.second();
		}
	}
}
