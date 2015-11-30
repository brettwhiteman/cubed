#include "game.h"
#include "world_gen/world_gen.h"
#include <thread>

Game::Game() :
	m_window{"Cubed", 800, 600, m_input_manager},
	m_rendering_engine(m_window),
	m_world{3},
	m_player{m_input_manager, WorldGen::get_spawn_pos()},
	m_physical_object_manager(m_world),
	m_running{true}
{
	m_rendering_engine.load_shader("basic_shader", {"position", "texCoord"}, {{UNIFORMTYPE_MAT4, "transform"}});
	m_rendering_engine.use_shader("basic_shader");
	
	m_rendering_engine.load_texture("blocks.png");
	m_rendering_engine.use_texture("blocks.png");

	m_input_manager.add_key_up_handler(InputManager::KEY_ESC, [this]()
	{
		m_running = !m_running;
		m_window.center_mouse();
	});

	m_physical_object_manager.add_object(&m_player);
}

void Game::run()
{
	#ifdef _DEBUG
		const auto TARGET_FPS = 30;
	#else
		const auto TARGET_FPS = 60;
	#endif

	const auto FRAME_DURATION = std::chrono::nanoseconds{std::nano::den / TARGET_FPS};
	std::chrono::nanoseconds unprocessed_time{0};
	auto last_update_time = std::chrono::steady_clock::now() - FRAME_DURATION;
	auto last_frame_time = std::chrono::steady_clock::now() - FRAME_DURATION;

	std::array<std::chrono::nanoseconds, 10> frame_times;
	decltype(frame_times)::size_type frame_times_index{0};

	frame_times.fill(FRAME_DURATION);

	auto frame_time_average = [&frame_times, &frame_times_index](std::chrono::nanoseconds latest)
	{
		frame_times[frame_times_index++] = latest;

		if (frame_times_index >= frame_times.size())
		{
			frame_times_index = 0;
		}

		std::remove_reference<decltype(frame_times)>::type::value_type::rep sum = 0;

		for (auto& frame_time : frame_times)
		{
			sum += frame_time.count();
		}

		return std::chrono::nanoseconds(sum / frame_times.size());
	};
	
	while (!m_input_manager.is_quit_requested())
	{
		auto now = std::chrono::steady_clock::now();
		unprocessed_time += now - last_update_time;
		last_update_time = now;
		bool needs_render = false;

		while (unprocessed_time >= FRAME_DURATION)
		{
			unprocessed_time -= FRAME_DURATION;
			needs_render = true;

			update(frame_time_average(now - last_frame_time));

			last_frame_time = now;
		}

		if (needs_render)
		{
			render();
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
}

void Game::update(std::chrono::nanoseconds delta)
{
	m_window.update(m_running);

	if (m_running)
	{
		m_player.update(delta);
		m_physical_object_manager.update(delta);
	}

	m_world.update(m_player.get_position());
	m_rendering_engine.set_mat4("transform", m_rendering_engine.get_projection_matrix() * m_player.get_camera().get_matrix());
	m_rendering_engine.update_uniforms();
}

void Game::render()
{
	m_rendering_engine.clear();
	m_world.render();
	m_window.swap_buffers();
}
