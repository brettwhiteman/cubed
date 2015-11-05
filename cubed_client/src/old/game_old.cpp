#include "game.h"
#include <glm/include/glm.hpp>
#include <glm/include/gtc/matrix_transform.hpp>
#include "world.h"
#include "blocks.h"
#include <thread>
#include "world_gen.h"
#include "transform.h"
#include "test_mob.h"
#include "inventory.h"
#include "items.h"
#include "globals.h"
#include "input_manager.h"

// Temporary until have settings
#define RENDER_DISTANCE 2

Game::Game(RenderingEngine& renderingEngine) :
	m_renderingEngine(renderingEngine), m_world(RENDER_DISTANCE), m_startTime(0), m_physics(16.0f), m_quit(false)
{
	Globals::game = this;

	m_startTime = getGameTime();

	// Init window size variables
	int ww, wh;
	m_renderingEngine.getWindowSize(&ww, &wh);
	m_windowMidX = ww / 2;
	m_windowMidY = wh / 2;

	// Centre the mouse
	m_renderingEngine.setMousePos(m_windowMidX, m_windowMidY);

	// Create the player object
	std::shared_ptr<Player> player = std::make_shared<Player>(WorldGen::getSpawnPos(), PI / 3.0f,
		static_cast<float>(ww) / wh, 0.05f, 1000.0f);
	m_namedPhysicalObjects.emplace("player", player);

	// Create the inventory object
	m_2DObjects.emplace("inventory", std::make_shared<Inventory>());

	// Create shaders
	m_shaders.emplace("basic", std::make_shared<BasicShader>());

	// Init the camera
	m_renderingEngine.m_pCamera = &player->m_camera;

	// Test mob
	//m_physicalObjects.emplace_back(new TestMob());
}

Game::~Game()
{
	
}

void Game::run()
{
	const float frameTime = 1000.0f / TARGET_FPS;
	const float tickTime = 1000.0f / TICKS_PER_SECOND;
	unsigned int now;
	float passedTime;
	float unprocessedTime = 0.0f;
	float untickedTime = 0.0f;
	unsigned int lastUpdateTime = getGameTime();
	bool renderNeeded;

	while(!m_renderingEngine.m_isCloseRequested && !m_quit)
	{
		now = getGameTime();
		passedTime = static_cast<float>(now - lastUpdateTime);
		lastUpdateTime = now;

		unprocessedTime += passedTime;
		untickedTime += passedTime;

		renderNeeded = false;

		while(untickedTime >= tickTime)
		{
			untickedTime -= tickTime;

			tick();
		}

		while(unprocessedTime >= frameTime)
		{
			unprocessedTime -= frameTime;

			update();

			renderNeeded = true;
		}

		if(renderNeeded)
			render();
		else
			Sleep(1);
	}
}

void Game::update()
{
	// Process Input
	InputManager::instance().update();

	m_physics.update(m_namedPhysicalObjects);
	m_physics.update(m_physicalObjects);

	// Update World
	m_world.update(*m_namedPhysicalObjects["player"]->m_pPos);

	// Update all physical objects
	for(auto& obj : m_namedPhysicalObjects)
		obj.second->update();

	for(auto& obj : m_physicalObjects)
		obj->update();
}

void Game::render()
{
	// 3D Rendering
	std::shared_ptr<Shader> basicShader = m_shaders["basic"];

	if(m_renderingEngine.m_pCamera)
		basicShader->setUniform("transform", m_renderingEngine.m_pCamera->getViewProjection());

	basicShader->bind();
	m_renderingEngine.clear();
	m_renderingEngine.setTexture(m_world.getTexture());
	m_world.draw();

	for(auto& obj : m_namedPhysicalObjects)
	{
		const Texture* pTexture = obj.second->getTexture();
		const Transform* pTransform = obj.second->getTransform();

		if(pTexture)
			m_renderingEngine.setTexture(*pTexture);

		if(pTransform && m_renderingEngine.m_pCamera)
		{
			glm::mat4 mat = m_renderingEngine.m_pCamera->getViewProjection() * pTransform->getModel();
			basicShader->setUniform("transform", mat);
		}

		obj.second->draw();
	}

	for(auto& obj : m_physicalObjects)
	{
		const Texture* pTexture = obj->getTexture();
		const Transform* pTransform = obj->getTransform();

		if(pTexture)
			m_renderingEngine.setTexture(*pTexture);

		if(pTransform && m_renderingEngine.m_pCamera)
		{
			glm::mat4 mat = m_renderingEngine.m_pCamera->getViewProjection() * pTransform->getModel();
			basicShader->setUniform("transform", mat);
			obj->draw();
			basicShader->setUniform("transform", m_renderingEngine.m_pCamera->getViewProjection());
		}
		else
		{
			obj->draw();
		}
	}

	// 2D Rendering

	basicShader->setUniform("transform", m_renderingEngine.m_2DTransform);

	for(auto& obj : m_2DObjects)
	{
		if(obj.second->render())
		{
			const Texture* pTexture = obj.second->getTexture();
			const Transform* pTransform = obj.second->getTransform();

			if(pTexture)
				m_renderingEngine.setTexture(*pTexture);

			if(pTransform)
			{
				glm::mat4 mat = m_renderingEngine.m_2DTransform * pTransform->getModel();
				basicShader->setUniform("transform", mat);
				obj.second->draw();
				basicShader->setUniform("transform", m_renderingEngine.m_2DTransform);
			}
			else
			{
				obj.second->draw();
			}
		}
	}
	
	m_renderingEngine.swapBuffers();
}

void Game::tick()
{
	for(auto& obj : m_namedPhysicalObjects)
		obj.second->tick();

	for(auto& obj : m_physicalObjects)
		obj->tick();
}