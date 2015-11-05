#ifndef _CHUNK_H_
#define _CHUNK_H_

#include "blocks.h"
#include <array>
#include "mesh_pti.h"
#include "world_constants.h"

class World;

class Chunk
{
	friend class WorldGen;
public:
	Chunk(int x, int y, int z);
	~Chunk();

	Chunk(const Chunk& chunk) = delete;

	void fill();
	void update();
	void draw();
	unsigned int getBlockIndex(int x, int y, int z) { return x * CHUNK_SIZE * CHUNK_SIZE + z * CHUNK_SIZE + y; }
	BlockID block(int x, int y, int z);
	void addBlock(int x, int y, int z, BlockID type);
	void removeBlock(int x, int y, int z);
	void flushBuffer();

	static World* pWorld;

	int m_x;
	int m_y;
	int m_z;

	bool m_dirty;
	bool m_updatePending;
	bool m_highPriorityUpdate;
	bool m_needsFill;

protected:
	std::array<BlockID, BLOCKS_PER_CHUNK> m_blocks;

private:
	bool m_needsMeshDataSet;
	std::array<Vertex_PT, BLOCKS_PER_CHUNK * VERTICES_PER_BLOCK>* m_verticesBuf;
	std::array<unsigned short, BLOCKS_PER_CHUNK * INDICES_PER_BLOCK>* m_indicesBuf;
	unsigned int m_verticesBufCount;
	unsigned int m_indicesBufCount;

	Mesh_PTI m_mesh;
};

#endif