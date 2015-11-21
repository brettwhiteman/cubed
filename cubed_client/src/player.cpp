#include "player.h"
#include <glm/include/gtc/constants.hpp>
#include <utility>
#include "input_manager.h"

Player::Player(glm::vec3 position) : m_camera(std::move(position), glm::pi<float>() / 3.0f, 1.333333f, 0.05f, 1000.0f)
{
}

void Player::update(InputManager& input)
{
	if (input.is_key_down(InputManager::KEY_A))
	{
		m_camera.move_right_relative(-0.5f);
	}

	if (input.is_key_down(InputManager::KEY_D))
	{
		m_camera.move_right_relative(0.5f);
	}

	if (input.is_key_down(InputManager::KEY_S))
	{
		m_camera.move_forward_relative(-0.5f);
	}

	if (input.is_key_down(InputManager::KEY_W))
	{
		m_camera.move_forward_relative(0.5f);
	}

	auto offset = input.get_mouse_offset();
	m_camera.rotate_yaw(static_cast<float>(offset.first) * -0.001f);
	m_camera.rotate_pitch(static_cast<float>(offset.second) * 0.001f);

	m_camera.update();
}