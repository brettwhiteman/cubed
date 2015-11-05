#include "blocks.h"
#include "block_air.h"
#include "block_grass.h"
#include "block_stone.h"
#include "block_dirt.h"
#include "block_bedrock.h"

Blocks::Blocks()
{
	registerBlockType<BlockAir>(BLOCK_AIR);
	registerBlockType<BlockGrass>(BLOCK_GRASS);
	registerBlockType<BlockStone>(BLOCK_STONE);
	registerBlockType<BlockDirt>(BLOCK_DIRT);
	registerBlockType<BlockBedrock>(BLOCK_BEDROCK);
}

Blocks& Blocks::instance()
{
	static Blocks blocks;

	return blocks;
}

template <typename T>
void Blocks::registerBlockType(BlockID id)
{
	auto it = m_blockTypes.find(id);

	if(it != m_blockTypes.end())
	{
		return;
	}

	m_blockTypes.emplace(id, std::shared_ptr<Block>(new T()));
}

std::shared_ptr<Block> Blocks::get(BlockID id)
{
	auto it = m_blockTypes.find(id);

	if(it == m_blockTypes.end())
	{
		return nullptr;
	}

	return it->second;
}
