#include "axis_aligned_bounding_box.h"
#include "physical_object.h"
#include "physical_object_manager.h"
#include "world.h"

void PhysicalObjectManager::update(std::chrono::nanoseconds delta)
{
	auto seconds = std::chrono::duration_cast<std::chrono::duration<float>>(delta);

	for (auto& object : m_objects)
	{
		auto bounding_box = object->m_bounding_box;
		auto collision = colliding_with_world(bounding_box);

		if (collision.first)
		{
			// TODO: Find out which component of velocity caused the collision
			// and use that to position the object hard against the block.

			object->handle_collision(std::get<0>(collision.second), std::get<1>(collision.second), std::get<2>(collision.second));
		}

		float speed = glm::length(object->m_velocity);

		if (speed < 0.001f)
		{
			object->m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		}
		else
		{
			if (!object->m_accelerating)
			{
				auto velocity = glm::normalize(object->m_velocity) * -object->m_natural_deceleration * seconds.count();

				if (glm::length(velocity) > speed)
				{
					object->m_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
				}
				else
				{
					object->m_velocity += velocity;
				}
			}

			float speed_to_max_ratio = speed / object->m_max_speed;

			if (speed_to_max_ratio > 1.0f)
			{
				object->m_velocity /= speed_to_max_ratio;
			}

			object->m_position += object->m_velocity * seconds.count();
		}
	}
}

std::pair<bool, std::tuple<int, int, int>> PhysicalObjectManager::colliding_with_world(const AxisAlignedBoundingBox& aabb)
{
	for (int x = static_cast<int>(aabb.low.x); x <= static_cast<int>(aabb.high.x); ++x)
	{
		for (int y = static_cast<int>(aabb.low.y); y <= static_cast<int>(aabb.high.y); ++y)
		{
			for (int z = static_cast<int>(aabb.low.z); z <= static_cast<int>(aabb.high.z); ++z)
			{
				if (m_world.is_block_at(x, y, z))
				{
					return std::make_pair(true, std::make_tuple(x, y, z));
				}
			}
		}
	}

	return std::make_pair(false, std::tuple<int, int, int>());
}