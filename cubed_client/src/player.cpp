#include "player.h"
#include <glm/include/gtc/constants.hpp>
#include <utility>
#include "input_manager.h"

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

void Player::update()
{
	if (m_input_manager.is_key_down(InputManager::KEY_A))
	{
		m_camera.move_right_relative(-0.05f);
	}

	if (m_input_manager.is_key_down(InputManager::KEY_D))
	{
		m_camera.move_right_relative(0.05f);
	}

	if (m_input_manager.is_key_down(InputManager::KEY_S))
	{
		m_camera.move_forward_relative(-0.05f);
	}

	if (m_input_manager.is_key_down(InputManager::KEY_W))
	{
		m_camera.move_forward_relative(0.05f);
	}

	m_camera.update();
}