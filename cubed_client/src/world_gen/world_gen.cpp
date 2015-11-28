#include "world_gen.h"
#include "noise.h"
#include "../world_constants.h"

namespace WorldGen
{
	const float TERRAIN_SPREAD = 16.0f;
	const int BASE_HEIGHT = 128;
	const float AMPLITUDE = 16.0f;

	void fill_chunk(BlockData& block_data, int start_x, int start_y, int start_z)
	{
		std::unique_lock<decltype(block_data.mutex)> lock(block_data.mutex);

		block_data.blocks.fill(BLOCK_AIR);

		lock.unlock();

		for (int x = start_x; x < start_x + WorldConstants::CHUNK_SIZE; ++x)
		{
			for (int z = start_z; z < start_z + WorldConstants::CHUNK_SIZE; ++z)
			{
				int height = get_height(x, z);

				for (int y = start_y; y < start_y + WorldConstants::CHUNK_SIZE; ++y)
				{
					if (y > height)
						break;

					auto block_index = Chunk::get_block_index(x - start_x, y - start_y, z - start_z);
					auto block_type = get_block_type(x, y, z, height);

					lock.lock();
					block_data.blocks[block_index] = block_type;
					lock.unlock();
				}
			}
		}
	}

	glm::vec3 get_spawn_pos()
	{
		return glm::vec3(0.0f, static_cast<float>(get_height(0, 0) + 3), 0.0f);
	}

	int get_height(int x, int z)
	{
		float _x = static_cast<float>(x) / TERRAIN_SPREAD;
		float _z = static_cast<float>(z) / TERRAIN_SPREAD;

		int fx = _x < 0.0f ? static_cast<int>(_x - 1.0f) : static_cast<int>(_x);
		int fz = _z < 0.0f ? static_cast<int>(_z - 1.0f) : static_cast<int>(_z);

		float height = bilinear_interpolate(
			noise_2d(fx, fz), noise_2d(fx + 1, fz), noise_2d(fx, fz + 1), noise_2d(fx + 1, fz + 1),
			_x - static_cast<float>(fx), _z - static_cast<float>(fz));

		return static_cast<int>(BASE_HEIGHT + height * AMPLITUDE);
	}

	BlockType get_block_type(int x, int y, int z, int height)
	{
		if (y < 40)
		{
			return BLOCK_STONE;
		}
		else if (height == y)
		{
			return BLOCK_GRASS;
		}
		else if (height - y < 7)
		{
			return BLOCK_DIRT;
		}
		else
		{
			return BLOCK_STONE;
		}
	}
}