#include "chunk_update.h"
#include "world.h"
#include "world_constants.h"
#include "world_gen/world_gen.h"

World* ChunkUpdate::s_world;

void ChunkUpdate::run()
{
	if (m_fill)
	{
		WorldGen::fill_chunk(*m_block_data, m_chunk_x * WorldConstants::CHUNK_SIZE, m_chunk_y * WorldConstants::CHUNK_SIZE, m_chunk_z * WorldConstants::CHUNK_SIZE);
	}
	
	int vi = 0;
	int ii = 0;

	for (int x = 0; x < WorldConstants::CHUNK_SIZE; ++x)
	{
		for (int z = 0; z < WorldConstants::CHUNK_SIZE; ++z)
		{
			for (int y = 0; y < WorldConstants::CHUNK_SIZE; ++y)
			{
				auto& props = s_world->get_block_properties(get_block_type(x, y, z));

				if (!props.render)
				{
					continue;
				}

				int abs_x = m_chunk_x * WorldConstants::CHUNK_SIZE + x;
				int abs_y = m_chunk_y * WorldConstants::CHUNK_SIZE + y;
				int abs_z = m_chunk_z * WorldConstants::CHUNK_SIZE + z;

				// Front Face
				if (!s_world->get_block_properties(get_block_type(x, y, z - 1)).render)
				{
					auto tex = props.tex_front;
					float tex_l = (static_cast<float>(tex.first) * WorldConstants::TEXTURE_STRIDE + WorldConstants::TEXTURE_PADDING) / WorldConstants::TEXTURE_ATLAS_SIZE;
					float tex_r = tex_l + static_cast<float>(WorldConstants::TEXTURE_SIZE) / WorldConstants::TEXTURE_ATLAS_SIZE;
					float tex_t = (static_cast<float>(tex.second) * WorldConstants::TEXTURE_STRIDE + WorldConstants::TEXTURE_PADDING) / WorldConstants::TEXTURE_ATLAS_SIZE;
					float tex_b = tex_t + static_cast<float>(WorldConstants::TEXTURE_SIZE) / WorldConstants::TEXTURE_ATLAS_SIZE;

					m_vertices[vi].position = glm::vec3(static_cast<float>(abs_x), static_cast<float>(abs_y), static_cast<float>(abs_z));
					m_vertices[vi].tex_coord = glm::vec2(tex_r, tex_b);
					m_vertices[vi + 1].position = glm::vec3(static_cast<float>(abs_x), static_cast<float>(abs_y + 1), static_cast<float>(abs_z));
					m_vertices[vi + 1].tex_coord = glm::vec2(tex_r, tex_t);
					m_vertices[vi + 2].position = glm::vec3(static_cast<float>(abs_x + 1), static_cast<float>(abs_y), static_cast<float>(abs_z));
					m_vertices[vi + 2].tex_coord = glm::vec2(tex_l, tex_b);
					m_vertices[vi + 3].position = glm::vec3(static_cast<float>(abs_x + 1), static_cast<float>(abs_y + 1), static_cast<float>(abs_z));
					m_vertices[vi + 3].tex_coord = glm::vec2(tex_l, tex_t);

					m_indices[ii++] = vi;
					m_indices[ii++] = vi + 1;
					m_indices[ii++] = vi + 2;
					m_indices[ii++] = vi + 2;
					m_indices[ii++] = vi + 1;
					m_indices[ii++] = vi + 3;

					vi += 4;
				}

				// Back Face
				if (!s_world->get_block_properties(get_block_type(x, y, z + 1)).render)
				{
					auto tex = props.tex_back;
					float tex_l = (static_cast<float>(tex.first) * WorldConstants::TEXTURE_STRIDE + WorldConstants::TEXTURE_PADDING) / WorldConstants::TEXTURE_ATLAS_SIZE;
					float tex_r = tex_l + static_cast<float>(WorldConstants::TEXTURE_SIZE) / WorldConstants::TEXTURE_ATLAS_SIZE;
					float tex_t = (static_cast<float>(tex.second) * WorldConstants::TEXTURE_STRIDE + WorldConstants::TEXTURE_PADDING) / WorldConstants::TEXTURE_ATLAS_SIZE;
					float tex_b = tex_t + static_cast<float>(WorldConstants::TEXTURE_SIZE) / WorldConstants::TEXTURE_ATLAS_SIZE;

					m_vertices[vi].position = glm::vec3(static_cast<float>(abs_x), static_cast<float>(abs_y), static_cast<float>(abs_z + 1));
					m_vertices[vi].tex_coord = glm::vec2(tex_l, tex_b);
					m_vertices[vi + 1].position = glm::vec3(static_cast<float>(abs_x + 1), static_cast<float>(abs_y), static_cast<float>(abs_z + 1));
					m_vertices[vi + 1].tex_coord = glm::vec2(tex_r, tex_b);
					m_vertices[vi + 2].position = glm::vec3(static_cast<float>(abs_x), static_cast<float>(abs_y + 1), static_cast<float>(abs_z + 1));
					m_vertices[vi + 2].tex_coord = glm::vec2(tex_l, tex_t);
					m_vertices[vi + 3].position = glm::vec3(static_cast<float>(abs_x + 1), static_cast<float>(abs_y + 1), static_cast<float>(abs_z + 1));
					m_vertices[vi + 3].tex_coord = glm::vec2(tex_r, tex_t);

					m_indices[ii++] = vi;
					m_indices[ii++] = vi + 1;
					m_indices[ii++] = vi + 2;
					m_indices[ii++] = vi + 2;
					m_indices[ii++] = vi + 1;
					m_indices[ii++] = vi + 3;

					vi += 4;
				}

				// Left Face
				if (!s_world->get_block_properties(get_block_type(x + 1, y, z)).render)
				{
					auto tex = props.tex_left;
					float tex_l = (static_cast<float>(tex.first) * WorldConstants::TEXTURE_STRIDE + WorldConstants::TEXTURE_PADDING) / WorldConstants::TEXTURE_ATLAS_SIZE;
					float tex_r = tex_l + static_cast<float>(WorldConstants::TEXTURE_SIZE) / WorldConstants::TEXTURE_ATLAS_SIZE;
					float tex_t = (static_cast<float>(tex.second) * WorldConstants::TEXTURE_STRIDE + WorldConstants::TEXTURE_PADDING) / WorldConstants::TEXTURE_ATLAS_SIZE;
					float tex_b = tex_t + static_cast<float>(WorldConstants::TEXTURE_SIZE) / WorldConstants::TEXTURE_ATLAS_SIZE;

					m_vertices[vi].position = glm::vec3(static_cast<float>(abs_x + 1), static_cast<float>(abs_y), static_cast<float>(abs_z));
					m_vertices[vi].tex_coord = glm::vec2(tex_r, tex_b);
					m_vertices[vi + 1].position = glm::vec3(static_cast<float>(abs_x + 1), static_cast<float>(abs_y + 1), static_cast<float>(abs_z));
					m_vertices[vi + 1].tex_coord = glm::vec2(tex_r, tex_t);
					m_vertices[vi + 2].position = glm::vec3(static_cast<float>(abs_x + 1), static_cast<float>(abs_y), static_cast<float>(abs_z + 1));
					m_vertices[vi + 2].tex_coord = glm::vec2(tex_l, tex_b);
					m_vertices[vi + 3].position = glm::vec3(static_cast<float>(abs_x + 1), static_cast<float>(abs_y + 1), static_cast<float>(abs_z + 1));
					m_vertices[vi + 3].tex_coord = glm::vec2(tex_l, tex_t);

					m_indices[ii++] = vi;
					m_indices[ii++] = vi + 1;
					m_indices[ii++] = vi + 2;
					m_indices[ii++] = vi + 2;
					m_indices[ii++] = vi + 1;
					m_indices[ii++] = vi + 3;

					vi += 4;
				}

				// Right Face
				if (!s_world->get_block_properties(get_block_type(x - 1, y, z)).render)
				{
					auto tex = props.tex_right;
					float tex_l = (static_cast<float>(tex.first) * WorldConstants::TEXTURE_STRIDE + WorldConstants::TEXTURE_PADDING) / WorldConstants::TEXTURE_ATLAS_SIZE;
					float tex_r = tex_l + static_cast<float>(WorldConstants::TEXTURE_SIZE) / WorldConstants::TEXTURE_ATLAS_SIZE;
					float tex_t = (static_cast<float>(tex.second) * WorldConstants::TEXTURE_STRIDE + WorldConstants::TEXTURE_PADDING) / WorldConstants::TEXTURE_ATLAS_SIZE;
					float tex_b = tex_t + static_cast<float>(WorldConstants::TEXTURE_SIZE) / WorldConstants::TEXTURE_ATLAS_SIZE;

					m_vertices[vi].position = glm::vec3(static_cast<float>(abs_x), static_cast<float>(abs_y), static_cast<float>(abs_z));
					m_vertices[vi].tex_coord = glm::vec2(tex_l, tex_b);
					m_vertices[vi + 1].position = glm::vec3(static_cast<float>(abs_x), static_cast<float>(abs_y), static_cast<float>(abs_z + 1));
					m_vertices[vi + 1].tex_coord = glm::vec2(tex_r, tex_b);
					m_vertices[vi + 2].position = glm::vec3(static_cast<float>(abs_x), static_cast<float>(abs_y + 1), static_cast<float>(abs_z));
					m_vertices[vi + 2].tex_coord = glm::vec2(tex_l, tex_t);
					m_vertices[vi + 3].position = glm::vec3(static_cast<float>(abs_x), static_cast<float>(abs_y + 1), static_cast<float>(abs_z + 1));
					m_vertices[vi + 3].tex_coord = glm::vec2(tex_r, tex_t);

					m_indices[ii++] = vi;
					m_indices[ii++] = vi + 1;
					m_indices[ii++] = vi + 2;
					m_indices[ii++] = vi + 2;
					m_indices[ii++] = vi + 1;
					m_indices[ii++] = vi + 3;

					vi += 4;
				}

				// Bottom Face
				if (!s_world->get_block_properties(get_block_type(x, y - 1, z)).render)
				{
					auto tex = props.tex_bottom;
					float tex_l = (static_cast<float>(tex.first) * WorldConstants::TEXTURE_STRIDE + WorldConstants::TEXTURE_PADDING) / WorldConstants::TEXTURE_ATLAS_SIZE;
					float tex_r = tex_l + static_cast<float>(WorldConstants::TEXTURE_SIZE) / WorldConstants::TEXTURE_ATLAS_SIZE;
					float tex_t = (static_cast<float>(tex.second) * WorldConstants::TEXTURE_STRIDE + WorldConstants::TEXTURE_PADDING) / WorldConstants::TEXTURE_ATLAS_SIZE;
					float tex_b = tex_t + static_cast<float>(WorldConstants::TEXTURE_SIZE) / WorldConstants::TEXTURE_ATLAS_SIZE;

					m_vertices[vi].position = glm::vec3(static_cast<float>(abs_x), static_cast<float>(abs_y), static_cast<float>(abs_z + 1));
					m_vertices[vi].tex_coord = glm::vec2(tex_r, tex_b);
					m_vertices[vi + 1].position = glm::vec3(static_cast<float>(abs_x), static_cast<float>(abs_y), static_cast<float>(abs_z));
					m_vertices[vi + 1].tex_coord = glm::vec2(tex_r, tex_t);
					m_vertices[vi + 2].position = glm::vec3(static_cast<float>(abs_x + 1), static_cast<float>(abs_y), static_cast<float>(abs_z + 1));
					m_vertices[vi + 2].tex_coord = glm::vec2(tex_l, tex_b);
					m_vertices[vi + 3].position = glm::vec3(static_cast<float>(abs_x + 1), static_cast<float>(abs_y), static_cast<float>(abs_z));
					m_vertices[vi + 3].tex_coord = glm::vec2(tex_l, tex_t);

					m_indices[ii++] = vi;
					m_indices[ii++] = vi + 1;
					m_indices[ii++] = vi + 2;
					m_indices[ii++] = vi + 2;
					m_indices[ii++] = vi + 1;
					m_indices[ii++] = vi + 3;

					vi += 4;
				}

				// Top Face
				if (!s_world->get_block_properties(get_block_type(x, y + 1, z)).render)
				{
					auto tex = props.tex_top;
					float tex_l = (static_cast<float>(tex.first) * WorldConstants::TEXTURE_STRIDE + WorldConstants::TEXTURE_PADDING) / WorldConstants::TEXTURE_ATLAS_SIZE;
					float tex_r = tex_l + static_cast<float>(WorldConstants::TEXTURE_SIZE) / WorldConstants::TEXTURE_ATLAS_SIZE;
					float tex_t = (static_cast<float>(tex.second) * WorldConstants::TEXTURE_STRIDE + WorldConstants::TEXTURE_PADDING) / WorldConstants::TEXTURE_ATLAS_SIZE;
					float tex_b = tex_t + static_cast<float>(WorldConstants::TEXTURE_SIZE) / WorldConstants::TEXTURE_ATLAS_SIZE;

					m_vertices[vi].position = glm::vec3(static_cast<float>(abs_x), static_cast<float>(abs_y + 1), static_cast<float>(abs_z + 1));
					m_vertices[vi].tex_coord = glm::vec2(tex_l, tex_t);
					m_vertices[vi + 1].position = glm::vec3(static_cast<float>(abs_x + 1), static_cast<float>(abs_y + 1), static_cast<float>(abs_z + 1));
					m_vertices[vi + 1].tex_coord = glm::vec2(tex_r, tex_t);
					m_vertices[vi + 2].position = glm::vec3(static_cast<float>(abs_x), static_cast<float>(abs_y + 1), static_cast<float>(abs_z));
					m_vertices[vi + 2].tex_coord = glm::vec2(tex_l, tex_b);
					m_vertices[vi + 3].position = glm::vec3(static_cast<float>(abs_x + 1), static_cast<float>(abs_y + 1), static_cast<float>(abs_z));
					m_vertices[vi + 3].tex_coord = glm::vec2(tex_r, tex_b);

					m_indices[ii++] = vi;
					m_indices[ii++] = vi + 1;
					m_indices[ii++] = vi + 2;
					m_indices[ii++] = vi + 2;
					m_indices[ii++] = vi + 1;
					m_indices[ii++] = vi + 3;

					vi += 4;
				}
			}
		}
	}

	m_num_vertices = vi;
	m_num_indices = ii;

	m_finished.store(true);
}

BlockType ChunkUpdate::get_block_type(int x, int y, int z) const
{
	if (x < 0 || x >= WorldConstants::CHUNK_SIZE || y < 0 || y >= WorldConstants::CHUNK_SIZE || z < 0 || z >= WorldConstants::CHUNK_SIZE)
	{
		return s_world->get_block_type(m_chunk_x * WorldConstants::CHUNK_SIZE + x, m_chunk_y * WorldConstants::CHUNK_SIZE + y, m_chunk_z * WorldConstants::CHUNK_SIZE + z);
	}

	auto block_index = Chunk::get_block_index(x, y, z);
	
	std::lock_guard<decltype(m_block_data->mutex)> lock(m_block_data->mutex);

	return m_block_data->blocks[block_index];
}