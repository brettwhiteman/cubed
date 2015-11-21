#ifndef CUBED_GAME_H
#define CUBED_GAME_H

#include "input_manager.h"
#include "window.h"
#include "rendering_engine.h"
#include "world.h"
#include "player.h"

class Game
{
public:
	Game();

	void run();

private:
	void update();
	void render();

	InputManager m_input;
	Window m_window;
	RenderingEngine m_rendering_engine;
	World m_world;
	Player m_player;
};

#endif