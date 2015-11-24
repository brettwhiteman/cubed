#ifndef CUBED_MESH_PTI_H
#define CUBED_MESH_PTI_H

#define GLEW_STATIC
#include <glew/include/glew.h>
#include <glm/include/glm.hpp>
#include <utility>

struct VertexPT
{
	VertexPT() { }
	VertexPT(glm::vec3 position, glm::vec2 tex_coord) : position(std::move(position)), tex_coord(std::move(tex_coord)) { }

	glm::vec3 position;
	glm::vec2 tex_coord;
};

class MeshPTI
{
public:
	MeshPTI(bool dynamic);
	MeshPTI(VertexPT vertices[], unsigned short indices[], GLsizei num_vertices, GLsizei num_indices, bool dynamic = false);
	MeshPTI(const MeshPTI&) = delete;
	~MeshPTI();

	void render() const;

	void set_data(const VertexPT vertices[], const unsigned short indices[], GLsizei num_vertices, GLsizei num_indices);
	void clear_data();

private:
	enum
	{
		VERTEX_BUFFER,
		INDEX_BUFFER,
		NUM_BUFFERS
	};

	GLuint m_vertex_arrays[1];
	GLuint m_buffers[NUM_BUFFERS];
	GLsizei m_num_vertices;
	GLsizei m_num_indices;
	bool m_dynamic;
	bool m_vbo;
	bool m_vao;
};

#endif