#ifndef CUBED_PLAYER_H
#define CUBED_PLAYER_H

#include "axis_aligned_bounding_box.h"
#include "camera.h"
#include "physical_object.h"
#include <chrono>
#include <glm/include/glm.hpp>

class InputManager;

class Player : public PhysicalObject
{
public:
	Player(InputManager& input_manager, glm::vec3 position);

	void update(std::chrono::nanoseconds delta);

	const glm::vec3& get_position() const { return m_position; }
	const Camera& get_camera() { return m_camera; }

private:
	InputManager& m_input_manager;
	Camera m_camera;

	static const float ACCELERATION;
};

#endif