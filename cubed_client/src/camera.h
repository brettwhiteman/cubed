#ifndef CUBED_CAMERA_H
#define CUBED_CAMERA_H

#include <algorithm>
#include <glm/include/glm.hpp>
#include <glm/include/gtx/rotate_vector.hpp>

class Camera
{
public:
	Camera(const glm::vec3& position, float fov, float aspect, float z_near, float z_far) :
		m_position(position),
		m_forward(0.0f, 0.0f, 1.0f),
		m_up(0.0f, 1.0f, 0.0f),
		m_yaw(0.0f),
		m_pitch(0.0f),
		MIN_PITCH(glm::pi<float>()  / -2.02f),
		MAX_PITCH(glm::pi<float>() / 2.02f),
		MAX_ANGLE(2.0f * glm::pi<float>())
	{
	}

	void update() { m_forward = glm::normalize(glm::rotate(glm::rotate(glm::vec3(0.0f, 0.0f, 1.0f), m_pitch, glm::vec3(1.0f, 0.0f, 0.0f)), m_yaw, glm::vec3(0.0f, 1.0f, 0.0f))); }

	void rotate_pitch(float angle)
	{
		m_pitch += angle;
		m_pitch = std::min(m_pitch, MAX_PITCH);
		m_pitch = std::max(m_pitch, MIN_PITCH);
	}

	void rotate_yaw(float angle)
	{
		m_yaw += angle;

		while (m_yaw > MAX_ANGLE)
		{
			m_yaw -= MAX_ANGLE;
		}

		while (m_yaw < 0.0f)
		{
			m_yaw += MAX_ANGLE;
		}
	}

	void set_position(const glm::vec3& position) { m_position = position; }

	glm::mat4 get_matrix() const { return glm::lookAt(m_position, m_position + m_forward, m_up); }
	glm::vec3 get_forward_vector() const { return m_forward; }
	glm::vec3 get_right_vector() const { return glm::normalize(glm::cross(m_forward, m_up)); }

private:
	glm::vec3 m_position;
	glm::vec3 m_forward;
	glm::vec3 m_up;
	float m_yaw;
	float m_pitch;
	
	const float MIN_PITCH;
	const float MAX_PITCH;
	const float MAX_ANGLE;
};

#endif