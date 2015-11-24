#ifndef CUBED_WORLD_CONSTANTS_H
#define CUBED_WORLD_CONSTANTS_H

namespace WorldConstants
{
	const int CHUNK_SIZE = 16;
	const int CHUNK_NUM_BLOCKS = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;
	const int VERTICES_PER_BLOCK = 24;
	const int INDICES_PER_BLOCK = 36;
	const int TEXTURE_STRIDE = 20;
	const int TEXTURE_PADDING = 2;
	const int TEXTURE_ATLAS_SIZE = 512;
	const int TEXTURE_SIZE = 16;
}

#endif