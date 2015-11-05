#include "input_manager.h"

input_manager::input_manager()
	: m_quit(false)
{
	m_key_states.fill(false);
	m_mouse_button_states.fill(false);
}

void input_manager::handle_key_down(key k)
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

void input_manager::handle_key_up(key k)
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

void input_manager::handle_mouse_down(mouse_button mb)
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

void input_manager::handle_mouse_up(mouse_button mb)
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
