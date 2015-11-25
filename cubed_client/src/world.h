#ifndef CUBED_WORLD_H
#define CUBED_WORLD_H

#include <unordered_map>
#include <memory>
#include <functional>
#include <array>
#include <mutex>
#include <thread>
#include <glm/include/glm.hpp>
#include "block_type.h"
#include "chunk_update.h"
#include "block_info.h"

class Chunk;

class World
{
public:
	World(int render_distance);
	~World();

	void update(const glm::vec3& center);
	void render();

	BlockType get_block_type(int block_x, int block_y, int block_z);

	auto& get_block_properties(BlockType type) { return m_block_info.get_properties(type); }

private:
	void chunk_update_thread();
	void update_loaded_chunks(const glm::vec3& center);
	void load_chunk(int chunk_x, int chunk_y, int chunk_z);
	Chunk* get_block_chunk(int block_x, int block_y, int block_z);
	Chunk* get_chunk(int chunk_x, int chunk_y, int chunk_z);
	void for_each_chunk(std::function<bool(Chunk*, int, int, int)> callback, bool skip_not_filled = true);

	int m_render_distance;
	std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::unique_ptr<Chunk>>>> m_chunks;
	std::array<std::unique_ptr<ChunkUpdate>, 10> m_chunk_updates;
	std::mutex m_chunk_updates_mutex;
	bool m_run_chunk_updates;
	std::thread m_chunk_update_thread;
	const BlockInfo m_block_info;
};

#endif