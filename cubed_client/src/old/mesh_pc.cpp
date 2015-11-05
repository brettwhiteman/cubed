#include "mesh_pc.h"

Mesh_PC::Mesh_PC(glm::vec3 positions[], glm::vec4 colors[], unsigned short numVertices, bool dynamic)
{
	glGenVertexArrays(1, m_vertexArrays);
	glBindVertexArray(m_vertexArrays[0]);

	glGenBuffers(NUM_BUFFERS, m_buffers);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(positions[0]), positions, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[COLOR_VB]);
	glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(colors[0]), colors, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindVertexArray(0);

	m_drawCount = numVertices;
}

Mesh_PC::~Mesh_PC()
{
	glDeleteBuffers(NUM_BUFFERS, m_buffers);
	glDeleteVertexArrays(1, m_vertexArrays);
}

void Mesh_PC::draw()
{
	glBindVertexArray(m_vertexArrays[0]);
	glDrawArrays(GL_TRIANGLES, 0, m_drawCount);
	glBindVertexArray(0);
}