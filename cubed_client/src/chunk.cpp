#include "chunk.h"
#include "world_gen/world_gen.h"
#include "world.h"

const Blocks Chunk::blocks;

Chunk::Chunk(int x, int y, int z)
	: m_x{x},
	m_y{y},
	m_z{z},
	m_filled{false},
	m_up_to_date{false},
	m_mesh{false}
{
	m_blocks.fill(BLOCK_AIR);
}

void Chunk::ensure_filled()
{
	if (m_filled)
	{
		return;
	}

	WorldGen::fill_chunk(*this);
	m_filled = true;
}

void Chunk::ensure_updated(World& world)
{
	if (m_up_to_date)
	{
		return;
	}

	ensure_filled();

	auto vertices = std::array<VertexPT, NUM_BLOCKS * VERTICES_PER_BLOCK>{};
	auto indices = std::array<unsigned short, NUM_BLOCKS * INDICES_PER_BLOCK>{};

	int vi = 0;
	int ii = 0;

	for (int x = 0; x < SIZE; ++x)
	{
		for (int z = 0; z < SIZE; ++z)
		{
			for (int y = 0; y < SIZE; ++y)
			{
				decltype(auto) props = blocks.get_properties(get_block_type(x, y, z));

				if (!props.render)
				{
					continue;
				}

				int abs_x = x + m_x * SIZE;
				int abs_y = y + m_y * SIZE;
				int abs_z = z + m_z * SIZE;

				// Front Face
				if (!blocks.get_properties(world.get_block_type(abs_x, abs_y, abs_z - 1)).render)
				{
					auto tex = props.tex_front;
					float tex_l = (static_cast<float>(tex.first) * TEXTURE_STRIDE + TEXTURE_PADDING) / TEXTURE_ATLAS_SIZE;
					float tex_r = tex_l + static_cast<float>(TEXTURE_SIZE) / TEXTURE_ATLAS_SIZE;
					float tex_t = (static_cast<float>(tex.second) * TEXTURE_STRIDE + TEXTURE_PADDING) / TEXTURE_ATLAS_SIZE;
					float tex_b = tex_t + static_cast<float>(TEXTURE_SIZE) / TEXTURE_ATLAS_SIZE;

					vertices[vi].position = glm::vec3(static_cast<float>(abs_x), static_cast<float>(abs_y), static_cast<float>(abs_z));
					vertices[vi].tex_coord = glm::vec2(tex_r, tex_b);
					vertices[vi + 1].position = glm::vec3(static_cast<float>(abs_x), static_cast<float>(abs_y + 1), static_cast<float>(abs_z));
					vertices[vi + 1].tex_coord = glm::vec2(tex_r, tex_t);
					vertices[vi + 2].position = glm::vec3(static_cast<float>(abs_x + 1), static_cast<float>(abs_y), static_cast<float>(abs_z));
					vertices[vi + 2].tex_coord = glm::vec2(tex_l, tex_b);
					vertices[vi + 3].position = glm::vec3(static_cast<float>(abs_x + 1), static_cast<float>(abs_y + 1), static_cast<float>(abs_z));
					vertices[vi + 3].tex_coord = glm::vec2(tex_l, tex_t);

					indices[ii++] = vi;
					indices[ii++] = vi + 1;
					indices[ii++] = vi + 2;
					indices[ii++] = vi + 2;
					indices[ii++] = vi + 1;
					indices[ii++] = vi + 3;

					vi += 4;
				}

				// Back Face
				if (!blocks.get_properties(world.get_block_type(abs_x, abs_y, abs_z + 1)).render)
				{
					auto tex = props.tex_back;
					float tex_l = (static_cast<float>(tex.first) * TEXTURE_STRIDE + TEXTURE_PADDING) / TEXTURE_ATLAS_SIZE;
					float tex_r = tex_l + static_cast<float>(TEXTURE_SIZE) / TEXTURE_ATLAS_SIZE;
					float tex_t = (static_cast<float>(tex.second) * TEXTURE_STRIDE + TEXTURE_PADDING) / TEXTURE_ATLAS_SIZE;
					float tex_b = tex_t + static_cast<float>(TEXTURE_SIZE) / TEXTURE_ATLAS_SIZE;

					vertices[vi].position = glm::vec3(static_cast<float>(abs_x), static_cast<float>(abs_y), static_cast<float>(abs_z + 1));
					vertices[vi].tex_coord = glm::vec2(tex_l, tex_b);
					vertices[vi + 1].position = glm::vec3(static_cast<float>(abs_x + 1), static_cast<float>(abs_y), static_cast<float>(abs_z + 1));
					vertices[vi + 1].tex_coord = glm::vec2(tex_r, tex_b);
					vertices[vi + 2].position = glm::vec3(static_cast<float>(abs_x), static_cast<float>(abs_y + 1), static_cast<float>(abs_z + 1));
					vertices[vi + 2].tex_coord = glm::vec2(tex_l, tex_t);
					vertices[vi + 3].position = glm::vec3(static_cast<float>(abs_x + 1), static_cast<float>(abs_y + 1), static_cast<float>(abs_z + 1));
					vertices[vi + 3].tex_coord = glm::vec2(tex_r, tex_t);

					indices[ii++] = vi;
					indices[ii++] = vi + 1;
					indices[ii++] = vi + 2;
					indices[ii++] = vi + 2;
					indices[ii++] = vi + 1;
					indices[ii++] = vi + 3;

					vi += 4;
				}

				// Left Face
				if (!blocks.get_properties(world.get_block_type(abs_x + 1, abs_y, abs_z)).render)
				{
					auto tex = props.tex_left;
					float tex_l = (static_cast<float>(tex.first) * TEXTURE_STRIDE + TEXTURE_PADDING) / TEXTURE_ATLAS_SIZE;
					float tex_r = tex_l + static_cast<float>(TEXTURE_SIZE) / TEXTURE_ATLAS_SIZE;
					float tex_t = (static_cast<float>(tex.second) * TEXTURE_STRIDE + TEXTURE_PADDING) / TEXTURE_ATLAS_SIZE;
					float tex_b = tex_t + static_cast<float>(TEXTURE_SIZE) / TEXTURE_ATLAS_SIZE;

					vertices[vi].position = glm::vec3(static_cast<float>(abs_x + 1), static_cast<float>(abs_y), static_cast<float>(abs_z));
					vertices[vi].tex_coord = glm::vec2(tex_r, tex_b);
					vertices[vi + 1].position = glm::vec3(static_cast<float>(abs_x + 1), static_cast<float>(abs_y + 1), static_cast<float>(abs_z));
					vertices[vi + 1].tex_coord = glm::vec2(tex_r, tex_t);
					vertices[vi + 2].position = glm::vec3(static_cast<float>(abs_x + 1), static_cast<float>(abs_y), static_cast<float>(abs_z + 1));
					vertices[vi + 2].tex_coord = glm::vec2(tex_l, tex_b);
					vertices[vi + 3].position = glm::vec3(static_cast<float>(abs_x + 1), static_cast<float>(abs_y + 1), static_cast<float>(abs_z + 1));
					vertices[vi + 3].tex_coord = glm::vec2(tex_l, tex_t);

					indices[ii++] = vi;
					indices[ii++] = vi + 1;
					indices[ii++] = vi + 2;
					indices[ii++] = vi + 2;
					indices[ii++] = vi + 1;
					indices[ii++] = vi + 3;

					vi += 4;
				}

				// Right Face
				if (!blocks.get_properties(world.get_block_type(abs_x - 1, abs_y, abs_z)).render)
				{
					auto tex = props.tex_right;
					float tex_l = (static_cast<float>(tex.first) * TEXTURE_STRIDE + TEXTURE_PADDING) / TEXTURE_ATLAS_SIZE;
					float tex_r = tex_l + static_cast<float>(TEXTURE_SIZE) / TEXTURE_ATLAS_SIZE;
					float tex_t = (static_cast<float>(tex.second) * TEXTURE_STRIDE + TEXTURE_PADDING) / TEXTURE_ATLAS_SIZE;
					float tex_b = tex_t + static_cast<float>(TEXTURE_SIZE) / TEXTURE_ATLAS_SIZE;

					vertices[vi].position = glm::vec3(static_cast<float>(abs_x), static_cast<float>(abs_y), static_cast<float>(abs_z));
					vertices[vi].tex_coord = glm::vec2(tex_l, tex_b);
					vertices[vi + 1].position = glm::vec3(static_cast<float>(abs_x), static_cast<float>(abs_y), static_cast<float>(abs_z + 1));
					vertices[vi + 1].tex_coord = glm::vec2(tex_r, tex_b);
					vertices[vi + 2].position = glm::vec3(static_cast<float>(abs_x), static_cast<float>(abs_y + 1), static_cast<float>(abs_z));
					vertices[vi + 2].tex_coord = glm::vec2(tex_l, tex_t);
					vertices[vi + 3].position = glm::vec3(static_cast<float>(abs_x), static_cast<float>(abs_y + 1), static_cast<float>(abs_z + 1));
					vertices[vi + 3].tex_coord = glm::vec2(tex_r, tex_t);

					indices[ii++] = vi;
					indices[ii++] = vi + 1;
					indices[ii++] = vi + 2;
					indices[ii++] = vi + 2;
					indices[ii++] = vi + 1;
					indices[ii++] = vi + 3;

					vi += 4;
				}

				// Bottom Face
				if (!blocks.get_properties(world.get_block_type(abs_x, abs_y - 1, abs_z)).render)
				{
					auto tex = props.tex_bottom;
					float tex_l = (static_cast<float>(tex.first) * TEXTURE_STRIDE + TEXTURE_PADDING) / TEXTURE_ATLAS_SIZE;
					float tex_r = tex_l + static_cast<float>(TEXTURE_SIZE) / TEXTURE_ATLAS_SIZE;
					float tex_t = (static_cast<float>(tex.second) * TEXTURE_STRIDE + TEXTURE_PADDING) / TEXTURE_ATLAS_SIZE;
					float tex_b = tex_t + static_cast<float>(TEXTURE_SIZE) / TEXTURE_ATLAS_SIZE;

					vertices[vi].position = glm::vec3(static_cast<float>(abs_x), static_cast<float>(abs_y), static_cast<float>(abs_z + 1));
					vertices[vi].tex_coord = glm::vec2(tex_r, tex_b);
					vertices[vi + 1].position = glm::vec3(static_cast<float>(abs_x), static_cast<float>(abs_y), static_cast<float>(abs_z));
					vertices[vi + 1].tex_coord = glm::vec2(tex_r, tex_t);
					vertices[vi + 2].position = glm::vec3(static_cast<float>(abs_x + 1), static_cast<float>(abs_y), static_cast<float>(abs_z + 1));
					vertices[vi + 2].tex_coord = glm::vec2(tex_l, tex_b);
					vertices[vi + 3].position = glm::vec3(static_cast<float>(abs_x + 1), static_cast<float>(abs_y), static_cast<float>(abs_z));
					vertices[vi + 3].tex_coord = glm::vec2(tex_l, tex_t);

					indices[ii++] = vi;
					indices[ii++] = vi + 1;
					indices[ii++] = vi + 2;
					indices[ii++] = vi + 2;
					indices[ii++] = vi + 1;
					indices[ii++] = vi + 3;

					vi += 4;
				}

				// Top Face
				if (!blocks.get_properties(world.get_block_type(abs_x, abs_y + 1, abs_z)).render)
				{
					auto tex = props.tex_top;
					float tex_l = (static_cast<float>(tex.first) * TEXTURE_STRIDE + TEXTURE_PADDING) / TEXTURE_ATLAS_SIZE;
					float tex_r = tex_l + static_cast<float>(TEXTURE_SIZE) / TEXTURE_ATLAS_SIZE;
					float tex_t = (static_cast<float>(tex.second) * TEXTURE_STRIDE + TEXTURE_PADDING) / TEXTURE_ATLAS_SIZE;
					float tex_b = tex_t + static_cast<float>(TEXTURE_SIZE) / TEXTURE_ATLAS_SIZE;

					vertices[vi].position = glm::vec3(static_cast<float>(abs_x), static_cast<float>(abs_y + 1), static_cast<float>(abs_z + 1));
					vertices[vi].tex_coord = glm::vec2(tex_l, tex_t);
					vertices[vi + 1].position = glm::vec3(static_cast<float>(abs_x + 1), static_cast<float>(abs_y + 1), static_cast<float>(abs_z + 1));
					vertices[vi + 1].tex_coord = glm::vec2(tex_r, tex_t);
					vertices[vi + 2].position = glm::vec3(static_cast<float>(abs_x), static_cast<float>(abs_y + 1), static_cast<float>(abs_z));
					vertices[vi + 2].tex_coord = glm::vec2(tex_l, tex_b);
					vertices[vi + 3].position = glm::vec3(static_cast<float>(abs_x + 1), static_cast<float>(abs_y + 1), static_cast<float>(abs_z));
					vertices[vi + 3].tex_coord = glm::vec2(tex_r, tex_b);

					indices[ii++] = vi;
					indices[ii++] = vi + 1;
					indices[ii++] = vi + 2;
					indices[ii++] = vi + 2;
					indices[ii++] = vi + 1;
					indices[ii++] = vi + 3;

					vi += 4;
				}
			}
		}

		m_mesh.set_data(vertices.data(), indices.data(), vi, ii);
		m_up_to_date = true;
	}
}