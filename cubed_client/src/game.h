#ifndef CUBED_GAME_H
#define CUBED_GAME_H

#include "input_manager.h"
#include "physical_object_manager.h"
#include "player.h"
#include "rendering_engine.h"
#include "window.h"
#include "world.h"
#include <chrono>

class Game
{
public:
	Game();

	void run();

private:
	void update(std::chrono::nanoseconds delta);
	void render();

	InputManager m_input_manager;
	Window m_window;
	RenderingEngine m_rendering_engine;
	World m_world;
	Player m_player;
	PhysicalObjectManager m_physical_object_manager;
	bool m_running;
};

#endif