#include "world.h"
#include "world_gen/world_gen.h"

world::world(int render_distance)
	: m_render_distance{render_distance < 1 ? 1 : render_distance}
{
	update_loaded_chunks(world_gen::get_spawn_pos());

	for (auto& x : m_loaded_chunks)
	{
		for (auto& y : x.second)
		{
			for (auto& z : y.second)
			{
				z.second->ensure_filled();
			}
		}
	}

	for (auto& x : m_loaded_chunks)
	{
		for (auto& y : x.second)
		{
			for (auto& z : y.second)
			{
				z.second->ensure_updated(*this);
			}
		}
	}
}

void world::update(const glm::vec3& center)
{
	update_loaded_chunks(center);

	for (auto& x : m_loaded_chunks)
	{
		for (auto& y : x.second)
		{
			for (auto& z : y.second)
			{
				z.second->ensure_updated(*this);
			}
		}
	}
}

void world::render()
{
	for (auto& x : m_loaded_chunks)
	{
		for (auto& y : x.second)
		{
			for (auto& z : y.second)
			{
				z.second->render();
			}
		}
	}
}

void world::update_loaded_chunks(const glm::vec3& center)
{
	int x = static_cast<int>((center.x < 0.0f) ? center.x - chunk::SIZE - 1.0f : center.x);
	int y = static_cast<int>((center.y < 0.0f) ? center.y - chunk::SIZE - 1.0f : center.y);
	int z = static_cast<int>((center.z < 0.0f) ? center.z - chunk::SIZE - 1.0f : center.z);

	x /= chunk::SIZE;
	y /= chunk::SIZE;
	z /= chunk::SIZE;

	int first_x = x - m_render_distance;
	int first_y = y - m_render_distance;
	int first_z = z - m_render_distance;

	int last_x = x + m_render_distance;
	int last_y = y + m_render_distance;
	int last_z = z + m_render_distance;

	for (x = first_x; x <= last_x; ++x)
	{
		for (y = first_y; y <= last_y; ++y)
		{
			for (z = first_z; z <= last_z; ++z)
			{
				if (!get_chunk(x, y, z))
					load_chunk(x, y, z);
			}
		}
	}

	bool del_x = false;
	bool del_y = false;

	for (auto it_x = m_loaded_chunks.begin(); it_x != m_loaded_chunks.end();)
	{
		for (auto it_y = it_x->second.begin(); it_y != it_x->second.end();)
		{
			for (auto it_z = it_y->second.begin(); it_z != it_y->second.end();)
			{
				if ((it_z->second->get_x() < first_x) || (it_z->second->get_x() > last_x))
				{
					del_x = true;

					break;
				}

				if ((it_z->second->get_y() < first_y) || (it_z->second->get_y() > last_y))
				{
					del_y = true;

					break;
				}

				if ((it_z->second->get_z() < first_z) || (it_z->second->get_z() > last_z))
				{
					it_z = it_y->second.erase(it_z);
				}
				else
					++it_z;
			}

			if (del_x)
			{
				break;
			}
			else if (del_y)
			{
				it_y = it_x->second.erase(it_y);
				del_y = false;
			}
			else
			{
				++it_y;
			}
		}

		if (del_x)
		{
			it_x = m_loaded_chunks.erase(it_x);
			del_x = false;
		}
		else
		{
			++it_x;
		}
	}
}

void world::load_chunk(int chunk_x, int chunk_y, int chunk_z)
{
	auto p1 = m_loaded_chunks.emplace(chunk_x, std::unordered_map<int, std::unordered_map<int, std::unique_ptr<chunk>>>{});
	auto p2 = p1.first->second.emplace(chunk_y, std::unordered_map<int, std::unique_ptr<chunk>>{});
	auto p3 = p2.first->second.emplace(chunk_z, std::make_unique<chunk>(chunk_x, chunk_y, chunk_z));

	chunk* chunk;

	chunk = get_chunk(chunk_x - 1, chunk_y, chunk_z);
	if (chunk)
	{
		chunk->set_dirty();
	}

	chunk = get_chunk(chunk_x + 1, chunk_y, chunk_z);
	if (chunk)
	{
		chunk->set_dirty();
	}

	chunk = get_chunk(chunk_x, chunk_y - 1, chunk_z);
	if (chunk)
	{
		chunk->set_dirty();
	}

	chunk = get_chunk(chunk_x, chunk_y + 1, chunk_z);
	if (chunk)
	{
		chunk->set_dirty();
	}

	chunk = get_chunk(chunk_x, chunk_y, chunk_z - 1);
	if (chunk)
	{
		chunk->set_dirty();
	}

	chunk = get_chunk(chunk_x, chunk_y, chunk_z + 1);
	if (chunk)
	{
		chunk->set_dirty();
	}
}

block_type world::get_block_type(int block_x, int block_y, int block_z)
{
	auto chunk = get_block_chunk(block_x, block_y, block_z);

	if (!chunk)
	{
		return BLOCK_AIR;
	}

	return chunk->get_block_type(static_cast<int>(block_x - chunk->get_x() * chunk::SIZE), static_cast<int>(block_y - chunk->get_y() * chunk::SIZE), static_cast<int>(block_z - chunk->get_z() * chunk::SIZE));
}

chunk* world::get_block_chunk(int block_x, int block_y, int block_z)
{
	int chunk_x = (block_x < 0 ? block_x - chunk::SIZE + 1 : block_x) / chunk::SIZE;
	int chunk_y = (block_y < 0 ? block_y - chunk::SIZE + 1 : block_y) / chunk::SIZE;
	int chunk_z = (block_z < 0 ? block_z - chunk::SIZE + 1 : block_z) / chunk::SIZE;

	return get_chunk(chunk_x, chunk_y, chunk_z);
}

chunk* world::get_chunk(int chunk_x, int chunk_y, int chunk_z)
{
	auto itx = m_loaded_chunks.find(chunk_x);

	if (itx == m_loaded_chunks.end())
	{
		return nullptr;
	}

	auto ity = itx->second.find(chunk_y);

	if (ity == itx->second.end())
	{
		return nullptr;
	}

	auto itz = ity->second.find(chunk_z);

	if (itz == ity->second.end())
	{
		return nullptr;
	}

	return itz->second.get();
}