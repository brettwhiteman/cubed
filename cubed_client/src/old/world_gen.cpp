#include "world_gen.h"

#define NOISE_MAGIC_X 93175
#define NOISE_MAGIC_Y 1
#define NOISE_MAGIC_Z 777

#define WORLD_TERRAIN_SPREAD 64.0f
#define WORLD_AMPLITUDE 20.0f
#define WORLD_BASE_HEIGHT 128.0f

int WorldGen::seed = 0;

void WorldGen::fillChunk(Chunk& chunk)
{
	int xStart = chunk.m_x * CHUNK_SIZE;
	int yStart = chunk.m_y * CHUNK_SIZE;
	int zStart = chunk.m_z * CHUNK_SIZE;

	for(int x = xStart; x < xStart + CHUNK_SIZE; ++x)
	{
		for(int z = zStart; z < zStart + CHUNK_SIZE; ++z)
		{
			int height = getHeight(x, z);

			for(int y = yStart; y < yStart + CHUNK_SIZE; ++y)
			{
				if(y > height)
					break;

				BlockID block = getBlockType(x, y, z, height);
				chunk.m_blocks[chunk.getBlockIndex(x - xStart, y - yStart, z - zStart)] = block;
			}

			/*for(int y = yStart; y < yStart + CHUNK_SIZE; ++y)
			{
				float h = getDensity(x, y, z);

				if(density > 0.0f)
				{
					unsigned int i = chunk.getBlockIndex(x - xStart, y - yStart, z - zStart);
					if(density < 0.05f)
						chunk.m_blocks[i] = BLOCK_GRASS;
					else if(density < 0.2f)
						chunk.m_blocks[i] = BLOCK_DIRT;
					else
						chunk.m_blocks[i] = BLOCK_STONE;
				}
			}*/
		}
	}
}

glm::vec3 WorldGen::getSpawnPos()
{
	return glm::vec3(0.0f, static_cast<float>(getHeight(0, 0) + 3), 0.0f);
}

float WorldGen::getDensity(int x, int y, int z)
{
	float _x = static_cast<float>(x) / WORLD_TERRAIN_SPREAD;
	float _y = static_cast<float>(y) / WORLD_TERRAIN_SPREAD;
	float _z = static_cast<float>(z) / WORLD_TERRAIN_SPREAD;

	int fx = _x < 0.0f ? static_cast<int>(_x - 1.0f) : static_cast<int>(_x);
	int fy = _y < 0.0f ? static_cast<int>(_y - 1.0f) : static_cast<int>(_y);
	int fz = _z < 0.0f ? static_cast<int>(_z - 1.0f) : static_cast<int>(_z);

	float density = trilinearInterpolate(
		noise3D(fx, fy, fz), noise3D(fx + 1, fy, fz), noise3D(fx, fy + 1, fz), noise3D(fx + 1, fy + 1, fz),
		noise3D(fx, fy, fz + 1), noise3D(fx + 1, fy, fz + 1), noise3D(fx, fy + 1, fz + 1), noise3D(fx + 1, fy + 1, fz + 1),
		_x - static_cast<float>(fx), _y - static_cast<float>(fy), _z - static_cast<float>(fz));

	return density;
}

int WorldGen::getHeight(int x, int z)
{
	float _x = static_cast<float>(x) / WORLD_TERRAIN_SPREAD;
	float _z = static_cast<float>(z) / WORLD_TERRAIN_SPREAD;

	int fx = _x < 0.0f ? static_cast<int>(_x - 1.0f) : static_cast<int>(_x);
	int fz = _z < 0.0f ? static_cast<int>(_z - 1.0f) : static_cast<int>(_z);

	float height = bilinearInterpolate(
		noise2D(fx, fz), noise2D(fx + 1, fz), noise2D(fx, fz + 1), noise2D(fx + 1, fz + 1),
		_x - static_cast<float>(fx), _z - static_cast<float>(fz));

	return static_cast<int>(WORLD_BASE_HEIGHT + height * WORLD_AMPLITUDE);
}

float WorldGen::noise3D(int x, int y, int z)
{
	int n = NOISE_MAGIC_X * x + NOISE_MAGIC_Y * y + NOISE_MAGIC_Z * z;
	n = (n >> 13) ^ n;
	n = (n * (n * n * 60493 + 19990303) + 1376312589) & 0x7fffffff;
	return 1.0f - (static_cast<float>(n) / 1073741824.0f);
}

float WorldGen::noise2D(int x, int y)
{
	int n = NOISE_MAGIC_X * x + NOISE_MAGIC_Z * y;
	n = (n >> 13) ^ n;
	n = (n * (n * n * 69313 + 263167) + 16785407) & 0x7fffffff;
	return 1.0f - (static_cast<float>(n) / 1073741824.0f);
}

BlockID WorldGen::getBlockType(int x, int y, int z, int height)
{
	if(y < 5)
	{
		//diamond level
		return BLOCK_STONE;
	}
	else if(y < 40)
	{
		float density = getDensity(x, y, z);

		if((density < 0.9f) && (density > 0.8f))
			return BLOCK_BEDROCK;

		return BLOCK_STONE;
	}
	else
	{
		if(height == y)
			return BLOCK_GRASS;
		else if(height - y < 7)
			return BLOCK_DIRT;
		else
			return BLOCK_STONE;
	}
}