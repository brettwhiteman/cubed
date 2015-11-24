#ifndef CUBED_CHUNK_H
#define CUBED_CHUNK_H

#include <array>
#include <memory>
#include "block_type.h"
#include "mesh_pti.h"
#include "world_constants.h"

class Chunk
{
public:
	Chunk(int x, int y, int z) :
		m_x{x},
		m_y{y},
		m_z{z},
		m_filled{false},
		m_up_to_date{false},
		m_update_queued{false},
		m_mesh{false},
		m_blocks{std::make_shared<BlockArray>()}
	{
	}

	void update_mesh(const VertexPT* vertices, const unsigned short* indices, GLsizei num_vertices, GLsizei num_indices) { m_mesh.set_data(vertices, indices, num_vertices, num_indices); }
	void render() const { m_mesh.render(); }

	auto filled() const { return m_filled; }
	auto up_to_date() const { return m_up_to_date; }
	auto update_queued() const { return m_update_queued; }
	void set_filled(bool filled) { m_filled = filled; }
	void set_up_to_date(bool up_to_date) { m_up_to_date = up_to_date; }
	void set_update_queued(bool update_queued) { m_update_queued = update_queued; }

	auto get_x() const { return m_x; }
	auto get_y() const { return m_y; }
	auto get_z() const { return m_z; }

	auto get_blocks() { return m_blocks; }

	auto get_block_type(int x, int y, int z) const { return m_filled ? (*m_blocks)[get_block_index(x, y, z)] : BLOCK_AIR; }
	void set_block_type(int x, int y, int z, BlockType type) { (*m_blocks)[get_block_index(x, y, z)] = type; }

	static int get_block_index(int x, int y, int z) { return x * WorldConstants::CHUNK_SIZE * WorldConstants::CHUNK_SIZE + z * WorldConstants::CHUNK_SIZE + y; }

	typedef std::array<BlockType, WorldConstants::CHUNK_NUM_BLOCKS> BlockArray;

private:
	int m_x;
	int m_y;
	int m_z;
	bool m_filled;
	bool m_up_to_date;
	bool m_update_queued;
	MeshPTI m_mesh;
	std::shared_ptr<BlockArray> m_blocks;
};

#endif