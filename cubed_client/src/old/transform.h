#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include <glm/include/glm.hpp>

class Transform
{
public:
	Transform(const glm::vec3& pos = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3& rot = glm::vec3(0.0f, 0.0f, 0.0f), const glm::vec3& scale = glm::vec3(1.0f, 1.0f, 1.0f));
	~Transform();

	glm::mat4 getModel() const;
	void setPos(const glm::vec3& pos);
	void setRot(const glm::vec3& rot);
	void setScale(const glm::vec3& scale);

	glm::vec3 m_pos;
	glm::vec3 m_rot;
	glm::vec3 m_scale;
};

#endif