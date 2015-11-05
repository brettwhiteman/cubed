#ifndef CUBED_WORLD_GEN_WORLD_GEN_H
#define CUBED_WORLD_GEN_WORLD_GEN_H

#include <glm/include/glm.hpp>
#include "../block_type.h"

class chunk;

namespace world_gen
{
	void fill_chunk(chunk& chunk);
	glm::vec3 get_spawn_pos();
	int get_height(int x, int z);
	block_type get_block_type(int x, int y, int z, int height);
}

#endif