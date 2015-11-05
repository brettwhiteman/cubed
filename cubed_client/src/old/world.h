#ifndef _WORLD_H_
#define _WORLD_H_

#include <unordered_map>
#include <memory>
#include <glm/include/glm.hpp>
#include "chunk.h"

class ChunkUpdate
{
public:
	ChunkUpdate(int priority, const std::tuple<int, int, int>& chunkPos) : m_priority(priority), m_chunkPos(chunkPos) { }

	int m_priority;
	std::tuple<int, int, int> m_chunkPos;

	inline bool operator<(const ChunkUpdate& other) const { return m_priority < other.m_priority; }
};

class World
{
public:
	World(unsigned int renderDistance);
	~World();

	World(const World& world) = delete;

	void update(const glm::vec3& playerPos);
	void render();
	void loadChunk(int x, int y, int z);
	std::shared_ptr<Chunk> getBlockChunk(int x, int y, int z);
	std::shared_ptr<Chunk> getChunk(int x, int y, int z);
	std::tuple<int, int, int> getBlockChunkPos(int x, int y, int z);
	BlockID getBlockID(int x, int y, int z);
	void addBlock(int x, int y, int z, BlockID type);
	void removeBlock(int x, int y, int z);
	void generateEntireWorld(const glm::vec3& playerPos);
	void updateLoadedChunks(const glm::vec3& playerPos);

	std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::shared_ptr<Chunk>>>> m_loadedChunks;
	unsigned int m_renderDistance;
};

#endif