#ifndef PHYSICALOBJECT_H
#define PHYSICALOBJECT_H

#include <glm/include/glm.hpp>

class Texture;
class Transform;

class PhysicalObject
{
public:
	PhysicalObject() : m_velocity(0.0f) {}
	virtual ~PhysicalObject() {};

	virtual void update() = 0;
	virtual void draw() = 0;
	virtual void tick() = 0;
	virtual const Texture* getTexture() = 0;
	virtual const Transform* getTransform() = 0;

	glm::vec3* m_pPos;
	glm::vec3 m_velocity;

	virtual float getWidth() = 0;
	virtual float getHeightBelow() = 0;
	virtual float getHeightAbove() = 0;
	virtual bool gravity() = 0;
};

#endif