#ifndef CUBED_GAME_H
#define CUBED_GAME_H

#include "input_manager.h"
#include "player.h"
#include "rendering_engine.h"
#include "window.h"
#include "world.h"

class Game
{
public:
	Game();

	void run();

private:
	void update();
	void render();

	InputManager m_input_manager;
	Window m_window;
	RenderingEngine m_rendering_engine;
	World m_world;
	Player m_player;
};

#endif