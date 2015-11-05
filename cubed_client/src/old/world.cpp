#include "world.h"
#include <Windows.h>
#include "input.h"
#include "world_gen.h"

using namespace std;

World::World(unsigned int renderDistance)
{
	m_renderDistance = renderDistance < 1 ? 1 : renderDistance;

	Chunk::pWorld = this;

	generateEntireWorld(WorldGen::getSpawnPos());
}

World::~World()
{

}

void World::update(const glm::vec3& playerPos)
{
	updateLoadedChunks(playerPos);
	
	int count = 0;

	for(auto& x : m_loadedChunks)
	{
		for(auto& y : x.second)
		{
			for(auto& z : y.second)
			{
				//if(z.second->m_dirty && (!z.second->m_updatePending))
				//	m_chunkUpdater.add(z.second);

				z.second->flushBuffer();
			}
		}
	}
}

void World::updateLoadedChunks(const glm::vec3& playerPos)
{
	// Update loaded chunks
	int x = static_cast<int>((playerPos.x < 0.0f) ? playerPos.x - static_cast<float>(CHUNK_SIZE)-1.0f : playerPos.x);
	int y = static_cast<int>((playerPos.y < 0.0f) ? playerPos.y - static_cast<float>(CHUNK_SIZE)-1.0f : playerPos.y);
	int z = static_cast<int>((playerPos.z < 0.0f) ? playerPos.z - static_cast<float>(CHUNK_SIZE)-1.0f : playerPos.z);

	x /= CHUNK_SIZE;
	y /= CHUNK_SIZE;
	z /= CHUNK_SIZE;

	int startX = x - m_renderDistance;
	int startY = y - m_renderDistance;
	int startZ = z - m_renderDistance;

	int endX = x + m_renderDistance;
	int endY = y + m_renderDistance;
	int endZ = z + m_renderDistance;

	// Ensure all chunks within render distance are loaded
	for(x = startX; x <= endX; ++x)
	{
		for(y = startY; y <= endY; ++y)
		{
			for(z = startZ; z <= endZ; ++z)
			{
				if(getChunk(x, y, z) == nullptr)
					loadChunk(x, y, z);
			}
		}
	}

	bool delX = false;
	bool delY = false;

	for(auto itX = m_loadedChunks.begin(); itX != m_loadedChunks.end();)
	{
		for(auto itY = itX->second.begin(); itY != itX->second.end();)
		{
			for(auto itZ = itY->second.begin(); itZ != itY->second.end();)
			{
				if((itZ->second->m_x < startX) || (itZ->second->m_x > endX))
				{
					delX = true;

					break;
				}

				if((itZ->second->m_y < startY) || (itZ->second->m_y > endY))
				{
					delY = true;

					break;
				}

				if((itZ->second->m_z < startZ) || (itZ->second->m_z > endZ))
				{
					itZ = itY->second.erase(itZ);
				}
				else
					++itZ;
			}

			if(delX)
				break;
			else if(delY)
			{
				itY = itX->second.erase(itY);
				delY = false;
			}
			else
				++itY;
		}

		if(delX)
		{
			itX = m_loadedChunks.erase(itX);
			delX = false;
		}
		else
			++itX;
	}
}

void World::render()
{
	for(auto& x : m_loadedChunks)
	{
		for(auto& y : x.second)
		{
			for(auto& z : y.second)
			{
				z.second->draw();
			}
		}
	}
}

void World::loadChunk(int x, int y, int z)
{
	auto p1 = m_loadedChunks.emplace(x, unordered_map<int, unordered_map<int, std::shared_ptr<Chunk>>>());
	auto p2 = p1.first->second.emplace(y, unordered_map<int, std::shared_ptr<Chunk>>());
	auto p3 = p2.first->second.emplace(z, std::make_shared<Chunk>(x, y, z));

	std::shared_ptr<Chunk> chunk;

	chunk = getChunk(x - 1, y, z);
	if(chunk)
	{
		chunk->m_highPriorityUpdate = false;
		chunk->m_dirty = true;
	}
	
	chunk = getChunk(x + 1, y, z);
	if(chunk)
	{
		chunk->m_highPriorityUpdate = false;
		chunk->m_dirty = true;
	}

	chunk = getChunk(x, y - 1, z);
	if(chunk)
	{
		chunk->m_highPriorityUpdate = false;
		chunk->m_dirty = true;
	}

	chunk = getChunk(x, y + 1, z);
	if(chunk)
	{
		chunk->m_highPriorityUpdate = false;
		chunk->m_dirty = true;
	}
	
	chunk = getChunk(x, y, z - 1);
	if(chunk)
	{
		chunk->m_highPriorityUpdate = false;
		chunk->m_dirty = true;
	}

	chunk = getChunk(x, y, z + 1);
	if(chunk)
	{
		chunk->m_highPriorityUpdate = false;
		chunk->m_dirty = true;
	}
}

std::shared_ptr<Chunk> World::getBlockChunk(int x, int y, int z)
{
	x = x < 0 ? x - CHUNK_SIZE + 1 : x;
	y = y < 0 ? y - CHUNK_SIZE + 1 : y;
	z = z < 0 ? z - CHUNK_SIZE + 1 : z;

	x /= CHUNK_SIZE;
	y /= CHUNK_SIZE;
	z /= CHUNK_SIZE;

	auto itx = m_loadedChunks.find(x);

	if(itx == m_loadedChunks.end())
		return nullptr;

	auto ity = itx->second.find(y);

	if(ity == itx->second.end())
		return nullptr;

	auto itz = ity->second.find(z);

	if(itz == ity->second.end())
		return nullptr;

	return itz->second;
}

BlockID World::getBlockID(int x, int y, int z)
{
	std::shared_ptr<Chunk> chunk = getBlockChunk(x, y, z);

	if(!chunk)
		return BLOCK_AIR;

	return chunk->block(static_cast<unsigned int>(x - chunk->m_x * CHUNK_SIZE),
						static_cast<unsigned int>(y - chunk->m_y * CHUNK_SIZE),
						static_cast<unsigned int>(z - chunk->m_z * CHUNK_SIZE));
}

std::shared_ptr<Chunk>World::getChunk(int x, int y, int z)
{
	auto itx = m_loadedChunks.find(x);

	if(itx == m_loadedChunks.end())
		return nullptr;

	auto ity = itx->second.find(y);

	if(ity == itx->second.end())
		return nullptr;

	auto itz = ity->second.find(z);

	if(itz == ity->second.end())
		return nullptr;

	return itz->second;
}

std::tuple<int, int, int> World::getBlockChunkPos(int x, int y, int z)
{
	x = x < 0 ? x - CHUNK_SIZE + 1 : x;
	y = y < 0 ? y - CHUNK_SIZE + 1 : y;
	z = z < 0 ? z - CHUNK_SIZE + 1 : z;

	x /= CHUNK_SIZE;
	y /= CHUNK_SIZE;
	z /= CHUNK_SIZE;

	return make_tuple(x, y, z);
}

void World::addBlock(int x, int y, int z, BlockID type)
{
	std::shared_ptr<Chunk> chunk = getBlockChunk(x, y, z);

	if(chunk != nullptr)
	{
		chunk->addBlock(x - chunk->m_x * CHUNK_SIZE, y - chunk->m_y * CHUNK_SIZE, z - chunk->m_z * CHUNK_SIZE, type);

		tuple<int, int, int> chunkPos = move(getBlockChunkPos(x, y, z));
		tuple<int, int, int> chunkPosBeside1 = move(getBlockChunkPos(x - 1, y, z));
		tuple<int, int, int> chunkPosBeside2 = move(getBlockChunkPos(x + 1, y, z));

		if((get<0>(chunkPosBeside1) != get<0>(chunkPos)))
		{
			chunk = getChunk(get<0>(chunkPosBeside1), get<1>(chunkPosBeside1), get<2>(chunkPosBeside1));

			if(chunk)
			{
				chunk->m_dirty = true;
				chunk->m_highPriorityUpdate = false;
			}
		}
		else if(get<0>(chunkPosBeside2) != get<0>(chunkPos))
		{
			chunk = getChunk(get<0>(chunkPosBeside2), get<1>(chunkPosBeside2), get<2>(chunkPosBeside2));

			if(chunk)
			{
				chunk->m_dirty = true;
				chunk->m_highPriorityUpdate = false;
			}
		}

		chunkPosBeside1 = move(getBlockChunkPos(x, y - 1, z));
		chunkPosBeside2 = move(getBlockChunkPos(x, y + 1, z));

		if((get<1>(chunkPosBeside1) != get<1>(chunkPos)))
		{
			chunk = getChunk(get<0>(chunkPosBeside1), get<1>(chunkPosBeside1), get<2>(chunkPosBeside1));

			if(chunk)
			{
				chunk->m_dirty = true;
				chunk->m_highPriorityUpdate = false;
			}
		}
		else if(get<1>(chunkPosBeside2) != get<1>(chunkPos))
		{
			chunk = getChunk(get<0>(chunkPosBeside2), get<1>(chunkPosBeside2), get<2>(chunkPosBeside2));

			if(chunk)
			{
				chunk->m_dirty = true;
				chunk->m_highPriorityUpdate = false;
			}
		}

		chunkPosBeside1 = move(getBlockChunkPos(x, y, z - 1));
		chunkPosBeside2 = move(getBlockChunkPos(x, y, z + 1));

		if((get<2>(chunkPosBeside1) != get<2>(chunkPos)))
		{
			chunk = getChunk(get<0>(chunkPosBeside1), get<1>(chunkPosBeside1), get<2>(chunkPosBeside1));

			if(chunk)
			{
				chunk->m_dirty = true;
				chunk->m_highPriorityUpdate = false;
			}
		}
		else if(get<2>(chunkPosBeside2) != get<2>(chunkPos))
		{
			chunk = getChunk(get<0>(chunkPosBeside2), get<1>(chunkPosBeside2), get<2>(chunkPosBeside2));

			if(chunk)
			{
				chunk->m_dirty = true;
				chunk->m_highPriorityUpdate = false;
			}
		}
	}
}

void World::removeBlock(int x, int y, int z)
{
	std::shared_ptr<Chunk> chunk = getBlockChunk(x, y, z);

	if(chunk != nullptr)
	{
		chunk->removeBlock(x - chunk->m_x * CHUNK_SIZE, y - chunk->m_y * CHUNK_SIZE, z - chunk->m_z * CHUNK_SIZE);

		tuple<int, int, int> chunkPos = move(getBlockChunkPos(x, y, z));
		tuple<int, int, int> chunkPosBeside1 = move(getBlockChunkPos(x - 1, y, z));
		tuple<int, int, int> chunkPosBeside2 = move(getBlockChunkPos(x + 1, y, z));

		if((get<0>(chunkPosBeside1) != get<0>(chunkPos)))
		{
			chunk = getChunk(get<0>(chunkPosBeside1), get<1>(chunkPosBeside1), get<2>(chunkPosBeside1));

			if(chunk)
			{
				chunk->m_dirty = true;
				chunk->m_highPriorityUpdate = false;
			}
		}
		else if(get<0>(chunkPosBeside2) != get<0>(chunkPos))
		{
			chunk = getChunk(get<0>(chunkPosBeside2), get<1>(chunkPosBeside2), get<2>(chunkPosBeside2));

			if(chunk)
			{
				chunk->m_dirty = true;
				chunk->m_highPriorityUpdate = false;
			}
		}

		chunkPosBeside1 = move(getBlockChunkPos(x, y - 1, z));
		chunkPosBeside2 = move(getBlockChunkPos(x, y + 1, z));

		if((get<1>(chunkPosBeside1) != get<1>(chunkPos)))
		{
			chunk = getChunk(get<0>(chunkPosBeside1), get<1>(chunkPosBeside1), get<2>(chunkPosBeside1));

			if(chunk)
			{
				chunk->m_dirty = true;
				chunk->m_highPriorityUpdate = false;
			}
		}
		else if(get<1>(chunkPosBeside2) != get<1>(chunkPos))
		{
			chunk = getChunk(get<0>(chunkPosBeside2), get<1>(chunkPosBeside2), get<2>(chunkPosBeside2));

			if(chunk)
			{
				chunk->m_dirty = true;
				chunk->m_highPriorityUpdate = false;
			}
		}

		chunkPosBeside1 = move(getBlockChunkPos(x, y, z - 1));
		chunkPosBeside2 = move(getBlockChunkPos(x, y, z + 1));

		if((get<2>(chunkPosBeside1) != get<2>(chunkPos)))
		{
			chunk = getChunk(get<0>(chunkPosBeside1), get<1>(chunkPosBeside1), get<2>(chunkPosBeside1));

			if(chunk)
			{
				chunk->m_dirty = true;
				chunk->m_highPriorityUpdate = false;
			}
		}
		else if(get<2>(chunkPosBeside2) != get<2>(chunkPos))
		{
			chunk = getChunk(get<0>(chunkPosBeside2), get<1>(chunkPosBeside2), get<2>(chunkPosBeside2));

			if(chunk)
			{
				chunk->m_dirty = true;
				chunk->m_highPriorityUpdate = false;
			}
		}
	}
}

void World::generateEntireWorld(const glm::vec3& playerPos)
{
	updateLoadedChunks(playerPos);

	for(auto& x : m_loadedChunks)
	{
		for(auto& y : x.second)
		{
			for(auto& z : y.second)
			{
				z.second->fill();
			}
		}
	}

	for(auto& x : m_loadedChunks)
	{
		for(auto& y : x.second)
		{
			for(auto& z : y.second)
			{
				z.second->update();
				z.second->flushBuffer();
			}
		}
	}
}
