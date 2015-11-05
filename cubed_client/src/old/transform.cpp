#include "transform.h"

#define GLM_FORCE_RADIANS
#include <glm/include/gtx/transform.hpp>

Transform::Transform(const glm::vec3& pos, const glm::vec3& rot, const glm::vec3& scale) : m_pos(pos), m_rot(rot), m_scale(scale)
{

}

Transform::~Transform()
{
}

glm::mat4 Transform::getModel() const
{
	glm::mat4 pos = glm::translate(m_pos);

	glm::mat4 rotX = glm::rotate(m_rot.x, glm::vec3(1.0f, 0.0f, 0.0f));
	glm::mat4 rotY = glm::rotate(m_rot.y, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 rot = rotY * rotX;
	
	glm::mat4 scale = glm::scale(m_scale);

	return pos;// *rot * scale;
}

void Transform::setPos(const glm::vec3& pos)
{
	m_pos = pos;
}

void Transform::setRot(const glm::vec3& rot)
{
	m_rot = rot;
}

void Transform::setScale(const glm::vec3& scale)
{
	m_scale = scale;
}