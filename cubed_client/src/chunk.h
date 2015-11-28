#ifndef CUBED_CHUNK_H
#define CUBED_CHUNK_H

#include "block_type.h"
#include "mesh_pti.h"
#include "world_constants.h"
#include <array>
#include <memory>
#include <mutex>

struct BlockData
{
	std::array<BlockType, WorldConstants::CHUNK_NUM_BLOCKS> blocks;
	std::mutex mutex;
};

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
		m_low_priority_update{false},
		m_mesh{false},
		m_block_data{std::make_shared<BlockData>()}
	{
	}

	void update_mesh(const VertexPT* vertices, const unsigned short* indices, GLsizei num_vertices, GLsizei num_indices) { m_mesh.set_data(vertices, indices, num_vertices, num_indices); }
	void render() const { m_mesh.render(); }

	auto filled() const { return m_filled; }
	auto up_to_date() const { return m_up_to_date; }
	auto update_queued() const { return m_update_queued; }
	auto low_priority_update() const { return m_low_priority_update; }
	void set_filled(bool filled) { m_filled = filled; }
	void set_up_to_date(bool up_to_date) { m_up_to_date = up_to_date; }
	void set_update_queued(bool update_queued) { m_update_queued = update_queued; }
	void set_low_priority_update(bool low_priority_update) { m_low_priority_update = low_priority_update; }

	auto get_x() const { return m_x; }
	auto get_y() const { return m_y; }
	auto get_z() const { return m_z; }

	auto get_block_data() { return m_block_data; }

	// A lock is only needed for writing. The chunk update thread does write to the block array
	// but only before m_filled is set to true. Once m_filled is true only the main thread
	// will write to the block array.
	auto get_block_type(int x, int y, int z) const { return m_filled ? m_block_data->blocks[get_block_index(x, y, z)] : BLOCK_AIR; }
	void set_block_type(int x, int y, int z, BlockType type)
	{
		auto block_index = get_block_index(x, y, z);

		std::lock_guard<decltype(m_block_data->mutex)> lock(m_block_data->mutex);
		m_block_data->blocks[block_index] = type;
	}

	static int get_block_index(int x, int y, int z) { return x * WorldConstants::CHUNK_SIZE * WorldConstants::CHUNK_SIZE + z * WorldConstants::CHUNK_SIZE + y; }

private:
	const int m_x;
	const int m_y;
	const int m_z;
	bool m_filled;
	bool m_up_to_date;
	bool m_update_queued;
	bool m_low_priority_update;
	MeshPTI m_mesh;
	const std::shared_ptr<BlockData> m_block_data;
};

#endif