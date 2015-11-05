#ifndef _WORLDGEN_H_
#define _WORLDGEN_H_

#include "chunk.h"

class WorldGen
{
public:
	static void fillChunk(Chunk& chunk);

	static float getDensity(int x, int y, int z);
	static int getHeight(int x, int z);
	static BlockID getBlockType(int x, int y, int z, int height);
	static glm::vec3 getSpawnPos();
	static float noise3D(int x, int y, int z);
	static float noise2D(int x, int y);
	static inline float linearInterpolate(float a, float b, float w)
	{
		return (1.0f - w) * a + w * b;
	}

	static inline float bilinearInterpolate(float p00, float p10, float p01, float p11, float wx, float wy)
	{
		float ix1 = linearInterpolate(p00, p10, wx);
		float ix2 = linearInterpolate(p01, p11, wx);
		return linearInterpolate(ix1, ix2, wy);
	}

	static inline float trilinearInterpolate(float p000, float p100, float p010, float p110, float p001, float p101, float p011, float p111, float wx, float wy, float wz)
	{
		float i1 = bilinearInterpolate(p000, p100, p010, p110, wx, wy);
		float i2 = bilinearInterpolate(p001, p101, p011, p111, wx, wy);
		return linearInterpolate(i1, i2, wz);
	}

	static int seed;
};

#endif