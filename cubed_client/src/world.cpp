#include "world.h"
#include "chunk.h"
#include "world_gen/world_gen.h"
#include "world_constants.h"

World::World(int render_distance) :
	m_render_distance{render_distance < 1 ? 1 : render_distance},
	m_run_chunk_updates{true}
{
	ChunkUpdate::set_world(this);
	update_loaded_chunks(WorldGen::get_spawn_pos());
	m_chunk_update_thread = std::thread{std::bind(&World::chunk_update_thread, this)};
}

World::~World()
{
	m_run_chunk_updates = false;

	if (m_chunk_update_thread.joinable())
	{
		m_chunk_update_thread.join();
	}
}

void World::update(const glm::vec3& center)
{
	update_loaded_chunks(center);

	for_each_chunk([this](Chunk* chunk, int x, int y, int z)
	{
		if (!chunk->update_queued() && (!chunk->filled() || !chunk->up_to_date()))
		{
			// Find a spare slot in the chunk updates array.
			// No need for read locking since only this thread writes to it.
			decltype(m_chunk_updates)::size_type chunk_update_slot = 0;

			for (; chunk_update_slot < m_chunk_updates.size(); ++chunk_update_slot)
			{
				if (!m_chunk_updates[chunk_update_slot])
				{
					break;
				}
			}

			if (chunk_update_slot == m_chunk_updates.size())
			{
				// Too many chunk updates queued up at the moment, end the for_each_chunk loop
				return false;
			}

			auto chunk_update = std::make_unique<ChunkUpdate>(chunk->get_block_data(), x, y, z, !chunk->filled());

			{
				std::lock_guard<decltype(m_chunk_updates_mutex)> lock(m_chunk_updates_mutex);
				m_chunk_updates[chunk_update_slot] = std::move(chunk_update);
			}

			chunk->set_update_queued(true);
		}

		return true;
	}, false);

	// Deferring the lock here as we don't need to lock for reading.
	// No other threads write to the list.
	std::unique_lock<decltype(m_chunk_updates_mutex)> lock(m_chunk_updates_mutex, std::defer_lock);

	for (auto& chunk_update : m_chunk_updates)
	{
		if (chunk_update && chunk_update->finished())
		{
			auto chunk = get_chunk(chunk_update->get_x(), chunk_update->get_y(), chunk_update->get_z());

			if (chunk)
			{
				chunk->update_mesh(chunk_update->get_vertices().data(), chunk_update->get_indices().data(), chunk_update->get_num_vertices(), chunk_update->get_num_indices());

				if (!chunk->filled())
				{
					// The chunk wasn't previously filled. Set all adjacent chunks
					// as not up to date since their meshes won't be optimized.
					// TODO: priority system for chunk updates, deprioritize these optimization updates

					Chunk* adjacent_chunk;

					adjacent_chunk = get_chunk(chunk->get_x() - 1, chunk->get_y(), chunk->get_z());
					if (adjacent_chunk)
					{
						adjacent_chunk->set_up_to_date(false);
					}

					adjacent_chunk = get_chunk(chunk->get_x() + 1, chunk->get_y(), chunk->get_z());
					if (adjacent_chunk)
					{
						adjacent_chunk->set_up_to_date(false);
					}

					adjacent_chunk = get_chunk(chunk->get_x(), chunk->get_y() - 1, chunk->get_z());
					if (adjacent_chunk)
					{
						adjacent_chunk->set_up_to_date(false);
					}

					adjacent_chunk = get_chunk(chunk->get_x(), chunk->get_y() + 1, chunk->get_z());
					if (adjacent_chunk)
					{
						adjacent_chunk->set_up_to_date(false);
					}

					adjacent_chunk = get_chunk(chunk->get_x(), chunk->get_y(), chunk->get_z() - 1);
					if (adjacent_chunk)
					{
						adjacent_chunk->set_up_to_date(false);
					}

					adjacent_chunk = get_chunk(chunk->get_x(), chunk->get_y(), chunk->get_z() + 1);
					if (adjacent_chunk)
					{
						adjacent_chunk->set_up_to_date(false);
					}
				}

				chunk->set_filled(true);
				chunk->set_up_to_date(true);
				chunk->set_update_queued(false);
			}

			lock.lock();
			chunk_update.reset();
			lock.unlock();

			// Only do one of these per frame to prevent stuttering
			break;
		}
	}
}

void World::render()
{
	for_each_chunk([](Chunk* chunk, int x, int y, int z)
	{
		chunk->render();
		return true;
	});
}

BlockType World::get_block_type(int block_x, int block_y, int block_z)
{
	auto chunk = get_block_chunk(block_x, block_y, block_z);

	if (!chunk)
	{
		return BLOCK_AIR;
	}

	return chunk->get_block_type(static_cast<int>(block_x - chunk->get_x() * WorldConstants::CHUNK_SIZE), static_cast<int>(block_y - chunk->get_y() * WorldConstants::CHUNK_SIZE), static_cast<int>(block_z - chunk->get_z() * WorldConstants::CHUNK_SIZE));
}

void World::chunk_update_thread()
{
	std::unique_lock<decltype(m_chunk_updates_mutex)> lock(m_chunk_updates_mutex, std::defer_lock);

	while (m_run_chunk_updates)
	{
		decltype(m_chunk_updates)::size_type i = 0;
		bool updates = false;

		lock.lock();

		for (auto& chunk_update : m_chunk_updates)
		{
			lock.unlock();

			if (chunk_update && !chunk_update->finished())
			{
				chunk_update->run();
				updates = true;
			}

			lock.lock();
		}

		lock.unlock();

		// Avoid hogging CPU
		if (!updates)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
}

void World::update_loaded_chunks(const glm::vec3& center)
{
	int x = static_cast<int>((center.x < 0.0f) ? center.x - WorldConstants::CHUNK_SIZE - 1.0f : center.x);
	int y = static_cast<int>((center.y < 0.0f) ? center.y - WorldConstants::CHUNK_SIZE - 1.0f : center.y);
	int z = static_cast<int>((center.z < 0.0f) ? center.z - WorldConstants::CHUNK_SIZE - 1.0f : center.z);

	x /= WorldConstants::CHUNK_SIZE;
	y /= WorldConstants::CHUNK_SIZE;
	z /= WorldConstants::CHUNK_SIZE;

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

	for (auto it_x = m_chunks.begin(); it_x != m_chunks.end();)
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
				{
					++it_z;
				}
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
			it_x = m_chunks.erase(it_x);
			del_x = false;
		}
		else
		{
			++it_x;
		}
	}
}

void World::load_chunk(int chunk_x, int chunk_y, int chunk_z)
{
	auto p1 = m_chunks.emplace(chunk_x, std::unordered_map<int, std::unordered_map<int, std::unique_ptr<Chunk>>>{});
	auto p2 = p1.first->second.emplace(chunk_y, std::unordered_map<int, std::unique_ptr<Chunk>>{});
	auto p3 = p2.first->second.emplace(chunk_z, std::make_unique<Chunk>(chunk_x, chunk_y, chunk_z));
}

Chunk* World::get_block_chunk(int block_x, int block_y, int block_z)
{
	int chunk_x = (block_x < 0 ? block_x - WorldConstants::CHUNK_SIZE + 1 : block_x) / WorldConstants::CHUNK_SIZE;
	int chunk_y = (block_y < 0 ? block_y - WorldConstants::CHUNK_SIZE + 1 : block_y) / WorldConstants::CHUNK_SIZE;
	int chunk_z = (block_z < 0 ? block_z - WorldConstants::CHUNK_SIZE + 1 : block_z) / WorldConstants::CHUNK_SIZE;

	return get_chunk(chunk_x, chunk_y, chunk_z);
}

Chunk* World::get_chunk(int chunk_x, int chunk_y, int chunk_z)
{
	auto itx = m_chunks.find(chunk_x);

	if (itx == m_chunks.end())
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

void World::for_each_chunk(std::function<bool(Chunk*, int, int, int)> callback, bool skip_not_filled)
{
	for (auto& x : m_chunks)
	{
		for (auto& y : x.second)
		{
			for (auto& z : y.second)
			{
				if (z.second->filled())
				{
					if (!callback(z.second.get(), x.first, y.first, z.first))
					{
						return;
					}
				}
				else if (!skip_not_filled && !callback(z.second.get(), x.first, y.first, z.first))
				{
					return;
				}
			}
		}
	}
}