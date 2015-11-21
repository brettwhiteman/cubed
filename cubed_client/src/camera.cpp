#include "camera.h"
#include <glm/include/gtx/rotate_vector.hpp>

Camera::Camera(glm::vec3 pos, float fov, float aspect, float z_near, float z_far)
	: m_pos(std::move(pos)),
	m_forward(0.0f, 0.0f, 1.0f),
	m_up(0.0f, 1.0f, 0.0f),
	m_yaw(0.0f),
	m_pitch(0.0f),
	m_projection(glm::perspective(fov, aspect, z_near, z_far))
{
}

glm::mat4 Camera::get_view_projection_matrix()
{
	return m_projection * glm::lookAt(m_pos, m_pos + m_forward, m_up);
}

void Camera::update()
{
	m_forward = glm::rotate(glm::rotate(glm::vec3(0.0f, 0.0f, 1.0f), m_pitch, glm::vec3(1.0f, 0.0f, 0.0f)), m_yaw, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Camera::move_right_relative(float amount)
{
	m_pos += glm::normalize(glm::cross(m_forward, m_up)) * amount;
}

void Camera::move_forward_relative(float amount)
{
	m_pos += m_forward * amount;
}

void Camera::move_up(float amount)
{
	m_pos += m_up * amount;
}

void Camera::rotate_pitch(float angle)
{
	m_pitch += angle;

	if (m_pitch > glm::pi<float>() / 2.02f)
	{
		m_pitch = glm::pi<float>() / 2.02f;
	}

	if (m_pitch < -glm::pi<float>() / 2.02f)
	{
		m_pitch = -glm::pi<float>() / 2.02f;
	}
}

void Camera::rotate_yaw(float angle)
{
	m_yaw += angle;

	while (m_yaw > glm::pi<float>() * 2.0f)
	{
		m_yaw -= 2.0f * glm::pi<float>();
	}

	while (m_yaw < 0.0f)
	{
		m_yaw += 2.0f * glm::pi<float>();
	}
}