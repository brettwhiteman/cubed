#include "input_manager.h"
#include "globals.h"
#include "game.h"

InputManager::InputManager() : m_state(STATE_NORMAL)
{
	m_keyStates.fill(false);
}

void InputManager::update()
{
	SDL_Event e;

	while(SDL_PollEvent(&e))
	{
		if(e.type == SDL_QUIT)
			Globals::game->m_quit = true;
		else if(e.type == SDL_KEYDOWN)
			handleKeyDownEvent(e.key.keysym.scancode);
		else if(e.type == SDL_KEYUP)
			handleKeyUpEvent(e.key.keysym.scancode);
		else if(e.type == SDL_MOUSEBUTTONDOWN)
			if(e.button.button == SDL_BUTTON_LEFT)
				handleLeftClick();
			else if(e.button.button == SDL_BUTTON_RIGHT)
				handleRightClick();
	}

	m_mouseState = SDL_GetMouseState(&m_mouseX, &m_mouseY);

	if(keyDown(SDL_SCANCODE_ESCAPE))
		Globals::game->m_quit = true;

	if(m_state == STATE_NORMAL)
		centreMouse();
}

std::pair<int, int> InputManager::getMouseOffset()
{
	int mouseOffsetX = m_mouseX - Globals::game->m_windowMidX;
	int mouseOffsetY = m_mouseY - Globals::game->m_windowMidY;

	return std::make_pair(mouseOffsetX, mouseOffsetY);
}

std::pair<int, int> InputManager::getMousePosition()
{
	return std::make_pair(m_mouseX, m_mouseY);
}

InputManager& InputManager::instance()
{
	static InputManager inputManager;
	return inputManager;
}

void InputManager::addKeyPressHandler(SDL_Scancode keyCode, std::function<void()> func)
{
	m_keyPressHandlers.push_back(Handler(keyCode, func));
}

void InputManager::addKeyReleaseHandler(SDL_Scancode keyCode, std::function<void()> func)
{
	m_keyReleaseHandlers.push_back(Handler(keyCode, func));
}

void InputManager::addLeftClickHandler(std::function<void()> func)
{
	m_leftClickHandlers.push_back(func);
}

void InputManager::addRightClickHandler(std::function<void()> func)
{
	m_rightClickHandlers.push_back(func);
}

void InputManager::handleKeyDownEvent(SDL_Scancode key)
{
	m_keyStates[key] = true;

	for(auto& h : m_keyPressHandlers)
	{
		if(h.code == key)
			h.func();
	}
}

void InputManager::handleKeyUpEvent(SDL_Scancode key)
{
	m_keyStates[key] = false;

	for(auto& h : m_keyReleaseHandlers)
	{
		if(h.code == key)
			h.func();
	}
}

void InputManager::centreMouse()
{
	Globals::game->m_renderingEngine.setMousePos(Globals::game->m_windowMidX, Globals::game->m_windowMidY);
}

void InputManager::handleLeftClick()
{
	for(auto& h : m_leftClickHandlers)
	{
		h();
	}
}

void InputManager::handleRightClick()
{
	for(auto& h : m_rightClickHandlers)
	{
		h();
	}
}