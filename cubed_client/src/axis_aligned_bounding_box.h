#ifndef CUBED_BOUNDING_BOX_H
#define CUBED_BOUNDING_BOX_H

#include <glm/include/glm.hpp>

class AxisAlignedBoundingBox
{
public:
	AxisAlignedBoundingBox() { }
	AxisAlignedBoundingBox(glm::vec3 low, glm::vec3 high) :
		low(low),
		high(high)
	{
	}

	bool intersects_with(const AxisAlignedBoundingBox& other) const
	{
		return contains_point(other.low) || contains_point(other.high) || other.contains_point(low) || other.contains_point(high);
	}

	bool contains_point(glm::vec3 point) const
	{
		return point.x >= low.x && point.x <= high.x && point.y >= low.y && point.y <= high.y && point.z >= low.z && point.z <= high.z;
	}

	glm::vec3 low;
	glm::vec3 high;
};

#endif