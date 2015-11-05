#include "physics.h"
#include "game.h"
#include "globals.h"

#define MAX_SPEED 50.0f

Physics::Physics(float gravity) : m_gravity(gravity)
{

}

Physics::~Physics()
{

}

void Physics::update(std::unordered_map<std::string, std::shared_ptr<PhysicalObject>>& namedObjects)
{
	for(auto& obj : namedObjects)
	{
		if(obj.second->gravity())
		{
			// Gravity
			accelerate(*obj.second, glm::vec3(0.0f, -m_gravity, 0.0f));
		}

		if(obj.second->m_velocity.x > MAX_SPEED)
			obj.second->m_velocity.x = MAX_SPEED;
		else if(obj.second->m_velocity.x < -MAX_SPEED)
			obj.second->m_velocity.x = -MAX_SPEED;

		if(obj.second->m_velocity.y > MAX_SPEED)
			obj.second->m_velocity.y = MAX_SPEED;
		else if(obj.second->m_velocity.y < -MAX_SPEED)
			obj.second->m_velocity.y = -MAX_SPEED;

		if(obj.second->m_velocity.z > MAX_SPEED)
			obj.second->m_velocity.z = MAX_SPEED;
		else if(obj.second->m_velocity.z < -MAX_SPEED)
			obj.second->m_velocity.z = -MAX_SPEED;

		glm::vec3 v = obj.second->m_velocity;

		v /= Globals::game->TARGET_FPS;

		obj.second->m_pPos->x += v.x;
		if(collidingWithWorld(*obj.second))
		{
			obj.second->m_pPos->x -= v.x;

			if(v.x < 0.0f)
			{
				float right = obj.second->m_pPos->x - obj.second->getWidth();
				int x = right < 0.0f ? static_cast<int>(right - 1.0f) : static_cast<int>(right);

				obj.second->m_pPos->x = static_cast<float>(x) + obj.second->getWidth() + 0.001f;
			}
			else
			{
				float left = obj.second->m_pPos->x + obj.second->getWidth();
				int x = left < 0.0f ? static_cast<int>(left - 1.0f) : static_cast<int>(left);

				obj.second->m_pPos->x = static_cast<float>(x + 1) - obj.second->getWidth() - 0.001f;
			}

			obj.second->m_velocity.x = 0.0f;
		}

		obj.second->m_pPos->y += v.y;
		if(collidingWithWorld(*obj.second))
		{
			obj.second->m_pPos->y -= v.y;

			if(v.y < 0.0f)
			{
				float bottom = obj.second->m_pPos->y - obj.second->getHeightBelow();
				int y = bottom < 0.0f ? static_cast<int>(bottom - 1.0f) : static_cast<int>(bottom);
				
				obj.second->m_pPos->y = static_cast<float>(y) + obj.second->getHeightBelow() + 0.001f;
			}
			else
			{
				float top = obj.second->m_pPos->y + obj.second->getHeightAbove();
				int y = top < 0.0f ? static_cast<int>(top - 1.0f) : static_cast<int>(top);

				obj.second->m_pPos->y = static_cast<float>(y + 1) - obj.second->getHeightAbove() - 0.001f;
			}

			obj.second->m_velocity.y = 0.0f;
		}

		obj.second->m_pPos->z += v.z;
		if(collidingWithWorld(*obj.second))
		{
			obj.second->m_pPos->z -= v.z;

			int z = obj.second->m_pPos->z < 0.0f ? static_cast<int>(obj.second->m_pPos->z - 1.0f) : static_cast<int>(obj.second->m_pPos->z);

			if(v.z < 0.0f)
			{
				float back = obj.second->m_pPos->z - obj.second->getWidth();
				int z = back < 0.0f ? static_cast<int>(back - 1.0f) : static_cast<int>(back);

				obj.second->m_pPos->z = static_cast<float>(z) + obj.second->getWidth() + 0.001f;
			}
			else
			{
				float forward = obj.second->m_pPos->z + obj.second->getWidth();
				int z = forward < 0.0f ? static_cast<int>(forward - 1.0f) : static_cast<int>(forward);

				obj.second->m_pPos->z = static_cast<float>(z + 1) - obj.second->getWidth() - 0.001f;
			}

			obj.second->m_velocity.z = 0.0f;
		}

		obj.second->m_velocity.x *= 0.9f;
		obj.second->m_velocity.z *= 0.9f;
	}
}

void Physics::update(std::vector<std::shared_ptr<PhysicalObject>>& objects)
{
	for(auto& obj : objects)
	{
		if(obj->gravity())
		{
			// Gravity
			accelerate(*obj, glm::vec3(0.0f, -m_gravity, 0.0f));
		}

		if(obj->m_velocity.x > MAX_SPEED)
			obj->m_velocity.x = MAX_SPEED;
		else if(obj->m_velocity.x < -MAX_SPEED)
			obj->m_velocity.x = -MAX_SPEED;

		if(obj->m_velocity.y > MAX_SPEED)
			obj->m_velocity.y = MAX_SPEED;
		else if(obj->m_velocity.y < -MAX_SPEED)
			obj->m_velocity.y = -MAX_SPEED;

		if(obj->m_velocity.z > MAX_SPEED)
			obj->m_velocity.z = MAX_SPEED;
		else if(obj->m_velocity.z < -MAX_SPEED)
			obj->m_velocity.z = -MAX_SPEED;

		glm::vec3 v = obj->m_velocity;

		v /= Globals::game->TARGET_FPS;

		obj->m_pPos->x += v.x;
		if(collidingWithWorld(*obj))
		{
			obj->m_pPos->x -= v.x;

			if(v.x < 0.0f)
			{
				float right = obj->m_pPos->x - obj->getWidth();
				int x = right < 0.0f ? static_cast<int>(right - 1.0f) : static_cast<int>(right);

				obj->m_pPos->x = static_cast<float>(x)+obj->getWidth() + 0.001f;
			} else
			{
				float left = obj->m_pPos->x + obj->getWidth();
				int x = left < 0.0f ? static_cast<int>(left - 1.0f) : static_cast<int>(left);

				obj->m_pPos->x = static_cast<float>(x + 1) - obj->getWidth() - 0.001f;
			}

			obj->m_velocity.x = 0.0f;
		}

		obj->m_pPos->y += v.y;
		if(collidingWithWorld(*obj))
		{
			obj->m_pPos->y -= v.y;

			if(v.y < 0.0f)
			{
				float bottom = obj->m_pPos->y - obj->getHeightBelow();
				int y = bottom < 0.0f ? static_cast<int>(bottom - 1.0f) : static_cast<int>(bottom);

				obj->m_pPos->y = static_cast<float>(y)+obj->getHeightBelow() + 0.001f;
			} else
			{
				float top = obj->m_pPos->y + obj->getHeightAbove();
				int y = top < 0.0f ? static_cast<int>(top - 1.0f) : static_cast<int>(top);

				obj->m_pPos->y = static_cast<float>(y + 1) - obj->getHeightAbove() - 0.001f;
			}

			obj->m_velocity.y = 0.0f;
		}

		obj->m_pPos->z += v.z;
		if(collidingWithWorld(*obj))
		{
			obj->m_pPos->z -= v.z;

			int z = obj->m_pPos->z < 0.0f ? static_cast<int>(obj->m_pPos->z - 1.0f) : static_cast<int>(obj->m_pPos->z);

			if(v.z < 0.0f)
			{
				float back = obj->m_pPos->z - obj->getWidth();
				int z = back < 0.0f ? static_cast<int>(back - 1.0f) : static_cast<int>(back);

				obj->m_pPos->z = static_cast<float>(z)+obj->getWidth() + 0.001f;
			} else
			{
				float forward = obj->m_pPos->z + obj->getWidth();
				int z = forward < 0.0f ? static_cast<int>(forward - 1.0f) : static_cast<int>(forward);

				obj->m_pPos->z = static_cast<float>(z + 1) - obj->getWidth() - 0.001f;
			}

			obj->m_velocity.z = 0.0f;
		}

		obj->m_velocity.x *= 0.9f;
		obj->m_velocity.z *= 0.9f;
	}
}

bool Physics::collidingWithWorld(PhysicalObject& object)
{
	float val = object.m_pPos->x - object.getWidth();
	const int xStart = val < 0.0f ? static_cast<int>(val - 1.0f) : static_cast<int>(val);
	val = object.m_pPos->x + object.getWidth();
	const int xEnd = val < 0.0f ? static_cast<int>(val - 1.0f) : static_cast<int>(val);
	val = object.m_pPos->y - object.getHeightBelow();
	const int yStart = val < 0.0f ? static_cast<int>(val - 1.0f) : static_cast<int>(val);
	val = object.m_pPos->y + object.getHeightAbove();
	const int yEnd = val < 0.0f ? static_cast<int>(val - 1.0f) : static_cast<int>(val);
	val = object.m_pPos->z - object.getWidth();
	const int zStart = val < 0.0f ? static_cast<int>(val - 1.0f) : static_cast<int>(val);
	val = object.m_pPos->z + object.getWidth();
	const int zEnd = val < 0.0f ? static_cast<int>(val - 1.0f) : static_cast<int>(val);

	for(int x = xStart; x <= xEnd; ++x)
	{
		for(int y = yStart; y <= yEnd; ++y)
		{
			for(int z = zStart; z <= zEnd; ++z)
			{
				if(Globals::game->m_world.getBlockID(x, y, z) != BLOCK_AIR)
					return true;
			}
		}
	}

	return false;
}

void Physics::accelerate(PhysicalObject& object, const glm::vec3& acceleration)
{
	object.m_velocity += acceleration / Globals::game->TARGET_FPS;
}
