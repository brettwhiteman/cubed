#ifndef _GAME_H_
#define _GAME_H_

#include <string>
#include <unordered_map>
#include <Windows.h>
#include "rendering_engine.h"
#include "physical_object.h"
#include <memory>
#include "world.h"
#include "player.h"
#include "physics.h"
#include "basic_shader.h"
#include "object_2d.h"

class Game
{
public:
	Game(RenderingEngine& renderingEngine);
	~Game();

	void run();
	void update();
	void render();
	void tick();

	// Components
	RenderingEngine& m_renderingEngine;
	World m_world;
	Physics m_physics;
	// ------------------

	// Public Variables
	int m_windowMidX;
	int m_windowMidY;
	bool m_quit;
	// ------------------

	// Objects
	std::unordered_map<std::string, std::shared_ptr<PhysicalObject>> m_namedPhysicalObjects;
	std::vector<std::shared_ptr<PhysicalObject>> m_physicalObjects;
	std::unordered_map<std::string, std::shared_ptr<Object2D>> m_2DObjects;
	std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaders;
	// ------------------

	// Constants
	const float TARGET_FPS = 60.0f;
	const int TICKS_PER_SECOND = 20;
	// ------------------

	// Timing
	inline unsigned int getGameTime() { return GetTickCount() - m_startTime; }
	unsigned int m_startTime;
	// ------------------
};

#endif