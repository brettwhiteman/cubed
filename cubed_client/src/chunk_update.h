#ifndef CUBED_CHUNK_UPDATE_H
#define CUBED_CHUNK_UPDATE_H

#include <memory>
#include <atomic>
#include "blocks.h"
#include "world_constants.h"
#include "chunk.h"

class ChunkUpdate
{
public:
	ChunkUpdate(std::shared_ptr<BlockData> block_data, int chunk_x, int chunk_y, int chunk_z, bool fill) :
		m_finished{false},
		m_block_data{std::move(block_data)},
		m_chunk_x{chunk_x},
		m_chunk_y{chunk_y},
		m_chunk_z{chunk_z},
		m_fill(fill)
	{
	}

	void run();

	auto finished() const { return m_finished.load(); }
	auto get_x() const { return m_chunk_x; }
	auto get_y() const { return m_chunk_y; }
	auto get_z() const { return m_chunk_z; }
	const auto& get_vertices() const { return m_vertices; }
	const auto& get_indices() const { return m_indices; }
	auto get_num_vertices() const { return m_num_vertices; }
	auto get_num_indices() const { return m_num_indices; }

private:
	BlockType get_block_type(int x, int y, int z) const;

	std::atomic_bool m_finished;
	std::shared_ptr<BlockData> m_block_data;
	int m_chunk_x;
	int m_chunk_y;
	int m_chunk_z;
	bool m_fill;
	std::array<VertexPT, WorldConstants::CHUNK_NUM_BLOCKS * WorldConstants::VERTICES_PER_BLOCK> m_vertices;
	std::array<unsigned short, WorldConstants::CHUNK_NUM_BLOCKS * WorldConstants::INDICES_PER_BLOCK> m_indices;
	GLsizei m_num_vertices;
	GLsizei m_num_indices;
};

#endif