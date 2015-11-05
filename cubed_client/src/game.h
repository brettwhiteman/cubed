#ifndef CUBED_GAME_H
#define CUBED_GAME_H

#include "input_manager.h"
#include "window.h"
#include "rendering_engine.h"
#include "world.h"
#include "player.h"

class game
{
public:
	game();

	void run();

private:
	void update();
	void render();

	input_manager m_input;
	window m_window;
	rendering_engine m_rendering_engine;
	world m_world;
	player m_player;
};

#endif