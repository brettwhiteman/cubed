#include "noise.h"

#define NOISE_MAGIC_X 93175
#define NOISE_MAGIC_Y 1
#define NOISE_MAGIC_Z 777

namespace world_gen
{
	float noise_2d(int x, int y)
	{
		int n = NOISE_MAGIC_X * x + NOISE_MAGIC_Z * y;
		n = (n >> 13) ^ n;
		n = (n * (n * n * 69313 + 263167) + 16785407) & 0x7fffffff;
		return 1.0f - (static_cast<float>(n) / 1073741824.0f);
	}

	float noise_3d(int x, int y, int z)
	{
		int n = NOISE_MAGIC_X * x + NOISE_MAGIC_Y * y + NOISE_MAGIC_Z * z;
		n = (n >> 13) ^ n;
		n = (n * (n * n * 60493 + 19990303) + 1376312589) & 0x7fffffff;
		return 1.0f - (static_cast<float>(n) / 1073741824.0f);
	}

	float linear_interpolate(float a, float b, float w)
	{
		return (1.0f - w) * a + w * b;
	}

	float bilinear_interpolate(float p00, float p10, float p01, float p11, float wx, float wy)
	{
		float ix1 = linear_interpolate(p00, p10, wx);
		float ix2 = linear_interpolate(p01, p11, wx);
		return linear_interpolate(ix1, ix2, wy);
	}

	float trilinear_interpolate(float p000, float p100, float p010, float p110, float p001, float p101, float p011, float p111, float wx, float wy, float wz)
	{
		float i1 = bilinear_interpolate(p000, p100, p010, p110, wx, wy);
		float i2 = bilinear_interpolate(p001, p101, p011, p111, wx, wy);
		return linear_interpolate(i1, i2, wz);
	}
}