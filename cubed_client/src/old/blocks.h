#ifndef BLOCKS_H
#define BLOCKS_H

#include "block.h"
#include <unordered_map>
#include <memory>

#define VERTICES_PER_BLOCK 24
#define INDICES_PER_BLOCK 36
#define TEXTURE_ATLAS_SIZE 512.0f
#define TEXTURE_SIZE 16.0f
#define TEXTURE_PADDING 2.0f
#define TEXTURE_STRIDE 20.0f

enum BlockID
{
	BLOCK_AIR,
	BLOCK_GRASS,
	BLOCK_STONE,
	BLOCK_DIRT,
	BLOCK_BEDROCK
};

class Blocks
{
public:
	Blocks();

	static Blocks& instance();

	template <typename T>
	void registerBlockType(BlockID id);

	std::shared_ptr<Block> get(BlockID id);

private:
	std::unordered_map<BlockID, std::shared_ptr<Block>> m_blockTypes;
};

#endif