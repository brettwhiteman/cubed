#ifndef CUBED_PHYSICAL_OBJECT_H
#define CUBED_PHYSICAL_OBJECT_H

#include "axis_aligned_bounding_box.h"
#include <glm/include/glm.hpp>

class PhysicalObject
{
	friend class PhysicalObjectManager;

public:
	PhysicalObject(const glm::vec3& position, const glm::vec3& velocity, float max_speed, float natural_deceleration) :
		PhysicalObject{position, velocity, max_speed, natural_deceleration, {{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}}}
	{
	}

	PhysicalObject(const glm::vec3& position, const glm::vec3& velocity, float max_speed, float natural_deceleration, AxisAlignedBoundingBox bounding_box) :
		m_position{position},
		m_velocity{velocity},
		m_max_speed{max_speed},
		m_natural_deceleration{natural_deceleration},
		m_accelerating{false},
		m_bounding_box{bounding_box}
	{
	}

	virtual ~PhysicalObject() { }

	virtual void handle_collision(PhysicalObject& object) { }
	virtual void handle_collision(int block_x, int block_y, int block_z) { }

protected:
	glm::vec3 m_position;
	glm::vec3 m_velocity;
	float m_max_speed;
	float m_natural_deceleration;
	bool m_accelerating;
	AxisAlignedBoundingBox m_bounding_box;
};

#endif