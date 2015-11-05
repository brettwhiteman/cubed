#include "game.h"
#include <chrono>
#include <Windows.h>
#include "world_gen/world_gen.h"

game::game()
	: m_window{"Cubed", 800, 600, m_input},
	m_rendering_engine(m_window),
	m_world{2},
	m_player{world_gen::get_spawn_pos()}
{
	m_rendering_engine.load_shader("basic_shader", {"position", "texCoord"}, {{UNIFORMTYPE_MAT4, "transform"}});
	m_rendering_engine.use_shader("basic_shader");
	
	m_rendering_engine.load_texture("blocks.png");
	m_rendering_engine.use_texture("blocks.png");

}

void game::run()
{
	const auto TARGET_FPS = 60;
	const auto FRAME_DURATION = std::chrono::nanoseconds{std::nano::den / TARGET_FPS};
	std::chrono::nanoseconds unprocessed_time{0};
	auto last_update_time = std::chrono::steady_clock::now() - FRAME_DURATION;

	while (!m_input.is_quit_requested())
	{
		auto now = std::chrono::steady_clock::now();
		unprocessed_time += now - last_update_time;
		last_update_time = now;
		bool needs_render = false;

		while (unprocessed_time >= FRAME_DURATION)
		{
			unprocessed_time -= FRAME_DURATION;
			needs_render = true;

			update();
		}

		if (needs_render)
		{
			render();
		}
		else
		{
			Sleep(1);
		}
	}
}

void game::update()
{
	m_window.update();
	m_player.update(m_input);
	m_world.update(world_gen::get_spawn_pos());// m_player.get_camera().get_position());
	m_rendering_engine.set_mat4("transform", m_player.get_camera().get_view_projection_matrix());
	m_rendering_engine.update_uniforms();
}

void game::render()
{
	m_rendering_engine.clear();
	m_world.render();
	m_window.swap_buffers();
}