#ifndef CUBED_WORLD_GEN_NOISE_H
#define CUBED_WORLD_GEN_NOISE_H

namespace WorldGen
{
	float noise_2d(int x, int y);
	float noise_3d(int x, int y, int z);
	float linear_interpolate(float a, float b, float w);
	float bilinear_interpolate(float p00, float p10, float p01, float p11, float wx, float wy);
	float trilinear_interpolate(float p000, float p100, float p010, float p110, float p001, float p101, float p011, float p111, float wx, float wy, float wz);
}

#endif