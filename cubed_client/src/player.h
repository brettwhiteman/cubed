#ifndef CUBED_PLAYER_H
#define CUBED_PLAYER_H

#include <glm/include/glm.hpp>
#include "camera.h"

class InputManager;

class Player
{
public:
	Player(glm::vec3 position);

	void update(InputManager& input);

	const glm::vec3& get_position() const { return m_camera.get_position(); }
	Camera& get_camera() { return m_camera; }

private:
	Camera m_camera;
};

#endif