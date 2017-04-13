#include "input_manager.h"
#include "player.h"
#include <glm/include/gtc/constants.hpp>
#include <utility>

const float Player::ACCELERATION = 30.0f;

Player::Player(InputManager& input_manager, glm::vec3 position) :
	PhysicalObject{position, {0.0f, 0.0f, 0.0f}, 10.0f, 35.0f},
	m_input_manager{input_manager},
	m_camera{std::move(position), glm::pi<float>() / 3.0f, 1.333333f, 0.05f, 1000.0f}
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

	glm::vec3 acceleration;

	if (m_input_manager.is_key_down(InputManager::KEY_A))
	{
		acceleration -= m_camera.get_right_vector();
	}

	if (m_input_manager.is_key_down(InputManager::KEY_D))
	{
		acceleration += m_camera.get_right_vector();
	}

	if (m_input_manager.is_key_down(InputManager::KEY_S))
	{
		acceleration -= m_camera.get_forward_vector();
	}

	if (m_input_manager.is_key_down(InputManager::KEY_W))
	{
		acceleration += m_camera.get_forward_vector();
	}

	if (glm::length(acceleration) > 0.001f)
	{
		acceleration = glm::normalize(acceleration) * ACCELERATION * seconds.count();
		m_velocity += acceleration;
		m_accelerating = true;
	}
	else
	{
		m_accelerating = false;
	}

	m_camera.set_position(m_position);

	m_camera.update();
}