#include "mesh_pti.h"

mesh_pti::mesh_pti(bool dynamic)
	: m_dynamic(dynamic),
	m_num_vertices(0),
	m_num_indices(0),
	m_vbo(false),
	m_vao(false)
{
}

mesh_pti::mesh_pti(vertex_pt vertices[], unsigned short indices[], GLsizei numVertices,	GLsizei numIndices, bool dynamic)
	: m_dynamic(dynamic), m_num_vertices(0), m_num_indices(0), m_vbo(false), m_vao(true)
{
	glGenVertexArrays(1, m_vertex_arrays);
	set_data(vertices, indices, numVertices, numIndices);
}

mesh_pti::~mesh_pti()
{
	if(m_vbo)
	{
		glDeleteBuffers(NUM_BUFFERS, m_buffers);
	}

	if(m_vao)
	{
		glDeleteVertexArrays(1, m_vertex_arrays);
	}
}

void mesh_pti::render() const
{
	if(m_num_indices > 0)
	{
		glBindVertexArray(m_vertex_arrays[0]);
		glDrawElements(GL_TRIANGLES, m_num_indices, GL_UNSIGNED_SHORT, 0);
	}
}

void mesh_pti::set_data(vertex_pt vertices[], unsigned short indices[], GLsizei numVertices, GLsizei numIndices)
{
	if(!m_vao)
	{
		glGenVertexArrays(1, m_vertex_arrays);
		m_vao = true;
	}

	glBindVertexArray(m_vertex_arrays[0]);

	if(!m_vbo)
	{
		glGenBuffers(NUM_BUFFERS, m_buffers);
		m_vbo = true;
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_buffers[VERTEX_BUFFER]);
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numVertices, vertices, m_dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

	GLsizei stride = sizeof(vertices[0].position) + sizeof(vertices[0].tex_coord);
	void* texCoordOffset = reinterpret_cast<void*>(sizeof(vertices[0].position));

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, texCoordOffset);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_buffers[INDEX_BUFFER]);
	
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numIndices, indices, m_dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

	m_num_vertices = numVertices;
	m_num_indices = numIndices;
}

void mesh_pti::clear_data()
{
	glDeleteBuffers(NUM_BUFFERS, m_buffers);
	m_num_vertices = 0;
	m_num_indices = 0;
	m_vbo = false;
}