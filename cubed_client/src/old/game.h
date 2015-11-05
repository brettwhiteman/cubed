#ifndef _GAME_H_
#define _GAME_H_

#include "input.h"
#include "window.h"
#include "rendering_engine.h"
#include "player.h"
#include "world.h"

class Game
{
public:
	Game();

	void run();

private:
	void update();
	void render();

	Input m_input;
	Window m_window;
	RenderingEngine m_renderingEngine;
	Player m_player;
	World m_world;
};

#endif