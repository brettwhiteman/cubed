#ifndef CUBED_PHYSICAL_OBJECT_MANAGER_H
#define CUBED_PHYSICAL_OBJECT_MANAGER_H

#include <chrono>
#include <utility>
#include <vector>

class AxisAlignedBoundingBox;
class PhysicalObject;
class World;

class PhysicalObjectManager
{
public:
	PhysicalObjectManager(const World& world) :
		m_world(world)
	{
	}

	void update(std::chrono::nanoseconds delta);
	void add_object(PhysicalObject* object) { m_objects.push_back(object); }

private:
	std::pair<bool, std::tuple<int, int, int>> colliding_with_world(const AxisAlignedBoundingBox& aabb);

	const World& m_world;
	std::vector<PhysicalObject*> m_objects;
};

#endif