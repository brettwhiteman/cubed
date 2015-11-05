#ifndef CUBED_WORLD_H
#define CUBED_WORLD_H

#include <unordered_map>
#include <memory>
#include <glm/include/glm.hpp>
#include "block_type.h"
#include "chunk.h"

class world
{
public:
	world(int render_distance);

	void update(const glm::vec3& center);
	void render();

	block_type get_block_type(int block_x, int block_y, int block_z);

private:
	void update_loaded_chunks(const glm::vec3& center);
	void load_chunk(int chunk_x, int chunk_y, int chunk_z);
	chunk* get_block_chunk(int block_x, int block_y, int block_z);
	chunk* get_chunk(int chunk_x, int chunk_y, int chunk_z);

	int m_render_distance;
	std::unordered_map<int, std::unordered_map<int, std::unordered_map<int, std::unique_ptr<chunk>>>> m_loaded_chunks;
};

#endif