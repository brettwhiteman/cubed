#ifndef TEST_MOB
#define TEST_MOB

#include "physical_object.h"
#include "texture.h"
#include "transform.h"
#include "mesh_pti.h"
#include <memory>

class TestMob : public PhysicalObject
{
public:
	TestMob();
	~TestMob();

	void update();
	void draw();
	void tick();
	inline const Texture* getTexture() { return &m_texture; }
	inline const Transform* getTransform() { return &m_transform; }

	inline float getWidth() { return 0.0f; }
	inline float getHeightBelow() { return 0.0f; }
	inline float getHeightAbove() { return 0.0f; }
	inline bool gravity() { return false; }

private:
	Texture m_texture;
	Transform m_transform;
	std::unique_ptr<Mesh_PTI> m_pMesh;
};

#endif