#include "input_manager.h"
#include "player.h"
#include <glm/include/gtc/constants.hpp>
#include <utility>

const float Player::MOVEMENT_SPEED = 4.0f;

Player::Player(InputManager& input_manager, glm::vec3 position) :
	m_input_manager(input_manager),
	m_camera(std::move(position), glm::pi<float>() / 3.0f, 1.333333f, 0.05f, 1000.0f)
{
	m_input_manager.add_mouse_move_handler([this](std::pair<int, int>& mouse_offset)
	{
		m_camera.rotate_yaw(static_cast<float>(mouse_offset.first) * -0.002f);
		m_camera.rotate_pitch(static_cast<float>(mouse_offset.second) * 0.002f);
	});
}

void Player::update(std::chrono::nanoseconds delta)
{
	auto seconds = std::chrono::duration_cast<std::chrono::duration<float>>(delta);

	glm::vec3 translation;
	bool movement = false;

	if (m_input_manager.is_key_down(InputManager::KEY_A))
	{
		translation -= m_camera.get_right_vector();
		movement = true;
	}

	if (m_input_manager.is_key_down(InputManager::KEY_D))
	{
		translation += m_camera.get_right_vector();
		movement = true;
	}

	if (m_input_manager.is_key_down(InputManager::KEY_S))
	{
		translation -= m_camera.get_forward_vector();
		movement = true;
	}

	if (m_input_manager.is_key_down(InputManager::KEY_W))
	{
		translation += m_camera.get_forward_vector();
		movement = true;
	}

	if (movement)
	{
		translation = glm::normalize(translation) * MOVEMENT_SPEED * seconds.count();
		m_camera.translate(translation);
	}

	m_camera.update();
}