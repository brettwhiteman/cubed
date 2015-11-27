#ifndef CUBED_PLAYER_H
#define CUBED_PLAYER_H

#include "camera.h"
#include <glm/include/glm.hpp>

class InputManager;

class Player
{
public:
	Player(InputManager& input_manager, glm::vec3 position);

	void update();

	const glm::vec3& get_position() const { return m_camera.get_position(); }
	Camera& get_camera() { return m_camera; }

private:
	InputManager& m_input_manager;
	Camera m_camera;
};

#endif