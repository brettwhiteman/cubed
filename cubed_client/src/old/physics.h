#ifndef _PHYSICS_H_
#define _PHYSICS_H_

#include "physical_object.h"
#include <vector>
#include <glm/include/glm.hpp>
#include <unordered_map>
#include <memory>

class Game;

class Physics
{
public:
	Physics(float gravity);
	~Physics();

	void update(std::unordered_map<std::string, std::shared_ptr<PhysicalObject>>& namedObjects);
	void update(std::vector<std::shared_ptr<PhysicalObject>>& objects);
	bool collidingWithWorld(PhysicalObject& object);
	void accelerate(PhysicalObject& object, const glm::vec3& acceleration);

private:
	float m_gravity;
};

#endif