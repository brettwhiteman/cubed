#include "game.h"
#include <chrono>
#include <Windows.h>
#include "world_gen.h"

Game::Game() : m_window("Cubed", 800, 600, m_input), m_player(WorldGen::getSpawnPos()), m_world(1)
{
	m_renderingEngine.loadShader("basic_shader", { "position", "texCoord" }, {{UniformType::UNIFORMTYPE_MAT4, "transform"}});
	m_renderingEngine.useShader("basic_shader");
	m_renderingEngine.loadTexture("blocks.png");
	m_renderingEngine.useTexture("blocks.png");
}

void Game::run()
{
	const int TARGET_FPS = 60;
	const auto FRAME_DURATION = std::chrono::nanoseconds(std::nano::den / TARGET_FPS);
	std::chrono::nanoseconds unprocessedTime(0);
	auto lastUpdateTime = std::chrono::steady_clock::now() - FRAME_DURATION;

	while (!m_input.isQuitRequested())
	{
		auto now = std::chrono::steady_clock::now();
		unprocessedTime += now - lastUpdateTime;
		lastUpdateTime = now;
		bool needsRender = false;

		while (unprocessedTime >= FRAME_DURATION)
		{
			unprocessedTime -= FRAME_DURATION;
			needsRender = true;

			update();
		}

		if (needsRender)
		{
			render();
		}
		else
		{
			Sleep(1);
		}
	}
}

void Game::update()
{
	m_window.update();
	m_world.update(m_player.getPosition());
}

void Game::render()
{
	m_renderingEngine.setMat4("transform", m_player.camera().getViewProjectionMatrix());
	m_renderingEngine.updateUniforms();
	m_renderingEngine.clear();
	m_world.render();
}