#ifndef CUBED_WORLD_GEN_WORLD_GEN_H
#define CUBED_WORLD_GEN_WORLD_GEN_H

#include <glm/include/glm.hpp>
#include "../chunk.h"

namespace WorldGen
{
	void fill_chunk(BlockData& block_data, int start_x, int start_y, int start_z);
	glm::vec3 get_spawn_pos();
	int get_height(int x, int z);
	BlockType get_block_type(int x, int y, int z, int height);
}

#endif