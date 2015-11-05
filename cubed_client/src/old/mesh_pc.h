#ifndef _MESH_PC_H_
#define _MESH_PC_H_

#define GLEW_STATIC
#include <glew/include/glew.h>
#include <glm/include/glm.hpp>

class Mesh_PC
{
public:
	Mesh_PC(glm::vec3 positions[], glm::vec4 colors[], unsigned short numVertices, bool dynamic = false);
	~Mesh_PC();

	void draw();

private:
	enum
	{
		POSITION_VB,
		COLOR_VB,

		NUM_BUFFERS
	};
	GLuint m_vertexArrays[1];
	GLuint m_buffers[NUM_BUFFERS];
	GLsizei m_drawCount;
};

#endif