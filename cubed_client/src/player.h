#ifndef CUBED_PLAYER_H
#define CUBED_PLAYER_H

#include <glm/include/glm.hpp>
#include "camera.h"

class input_manager;

class player
{
public:
	player(glm::vec3 position);

	void update(input_manager& input);

	const glm::vec3& get_position() const { return m_camera.get_position(); }
	camera& get_camera() { return m_camera; }

private:
	camera m_camera;
};

#endif