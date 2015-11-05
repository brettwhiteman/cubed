#ifndef CUBED_CAMERA_H
#define CUBED_CAMERA_H

#include <glm/include/glm.hpp>

class camera
{
public:
	camera(glm::vec3 pos, float fov, float aspect, float zNear, float zFar);

	glm::mat4 get_view_projection_matrix();
	void update();
	void move_right_relative(float amount);
	void move_forward_relative(float amount);
	void move_up(float amount);
	void rotate_pitch(float angle);
	void rotate_yaw(float angle);

	glm::vec3 get_walk_forward_vector() { return glm::normalize(glm::vec3(m_forward.x, 0.0f, m_forward.z)); }
	glm::vec3 get_walk_right_vector() { return glm::normalize(glm::cross(m_forward, m_up)); }

	const glm::vec3& get_position() const { return m_pos; }

private:
	glm::vec3 m_pos;
	glm::vec3 m_forward;
	glm::vec3 m_up;
	float m_yaw;
	float m_pitch;
	glm::mat4 m_projection;
};

#endif