#include "test_mob.h"
#include "globals.h"
#include "game.h"

TestMob::TestMob() : m_texture("res\\testmob.png")
{
	m_pPos = &m_transform.m_pos;
	Vertex_PT vertices[] =
	{
		{{0.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
		{{0.0f, -1.0f, 2.0f}, {0.0f, 1.0f}},
		{{2.0f, -1.0f, 0.0f}, {1.0f, 0.0f}}
	};

	unsigned short indices[] = {0, 1, 2};

	m_pMesh.reset(new Mesh_PTI(vertices, indices, 3, 3));
}

TestMob::~TestMob()
{

}

void TestMob::update()
{
	glm::vec3 playerPos = *Globals::game->m_namedPhysicalObjects["player"]->m_pPos;

	m_transform.setPos(playerPos);
}

void TestMob::draw()
{
	m_pMesh->draw();
}

void TestMob::tick()
{

}