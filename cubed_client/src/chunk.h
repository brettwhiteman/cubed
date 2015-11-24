#ifndef CUBED_CHUNK_H
#define CUBED_CHUNK_H

#include <array>
#include "block_type.h"
#include "mesh_pti.h"
#include "blocks.h"

class World;

class Chunk
{
public:
	Chunk(int x, int y, int z);

	void fill();
	bool update(World& world);
	void set_dirty() { m_up_to_date = false; }
	void render() const { m_mesh.render(); }

	auto get_x() const { return m_x; }
	auto get_y() const { return m_y; }
	auto get_z() const { return m_z; }
	auto empty() const { return !m_filled; }

	auto get_block_type(int x, int y, int z) const { return m_filled ? m_blocks[get_block_index(x, y, z)] : BLOCK_AIR; }
	void set_block_type(int x, int y, int z, BlockType type) { m_blocks[get_block_index(x, y, z)] = type; }

	static int get_block_index(int x, int y, int z) { return x * SIZE * SIZE + z * SIZE + y; }

	static const int SIZE = 16;
	static const int NUM_BLOCKS = SIZE * SIZE * SIZE;
	static const int VERTICES_PER_BLOCK = 24;
	static const int INDICES_PER_BLOCK = 36;
	static const int TEXTURE_STRIDE = 20;
	static const int TEXTURE_PADDING = 2;
	static const int TEXTURE_ATLAS_SIZE = 512;
	static const int TEXTURE_SIZE = 16;

private:
	int m_x;
	int m_y;
	int m_z;
	std::array<BlockType, NUM_BLOCKS> m_blocks;
	bool m_filled;
	bool m_up_to_date;
	MeshPTI m_mesh;

	static const Blocks blocks;
};

#endif