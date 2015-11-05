#include "chunk.h"
#include "world_gen.h"
#include <memory>
#include "world.h"

World* Chunk::pWorld = 0;

Chunk::Chunk(int x, int y, int z) : m_x(x), m_y(y), m_z(z), m_mesh(false), m_dirty(true), m_highPriorityUpdate(false), m_needsFill(true),
	m_verticesBuf(nullptr), m_indicesBuf(nullptr), m_verticesBufCount(0), m_indicesBufCount(0)
{
	m_blocks.fill(BLOCK_AIR);
}

Chunk::~Chunk()
{
	if(m_verticesBuf)
		delete m_verticesBuf;

	if(m_indicesBuf)
		delete m_indicesBuf;
}

void Chunk::fill()
{
	m_blocks.fill(BLOCK_AIR);

	WorldGen::fillChunk(*this);

	m_dirty = true;
	m_needsFill = false;
}

void Chunk::update()
{
	if(m_needsFill)
		fill();

	if(m_verticesBuf)
		delete m_verticesBuf;
	if(m_indicesBuf)
		delete m_indicesBuf;

	m_verticesBuf = new std::array<Vertex_PT, BLOCKS_PER_CHUNK * VERTICES_PER_BLOCK>();
	m_indicesBuf = new std::array<unsigned short, BLOCKS_PER_CHUNK * INDICES_PER_BLOCK>();
	auto& vertices = *m_verticesBuf;
	auto& indices = *m_indicesBuf;

	unsigned int vi = 0;
	unsigned int ii = 0;

	for(unsigned int x = 0; x < CHUNK_SIZE; ++x)
	{
		for(unsigned int z = 0; z < CHUNK_SIZE; ++z)
		{
			for(unsigned int y = 0; y < CHUNK_SIZE; ++y)
			{
				BlockID type;
				type = m_blocks[getBlockIndex(x, y, z)];

				std::shared_ptr<Block> block = Blocks::instance().get(type);

				if(!block->render())
					continue;

				int absX = x + m_x * CHUNK_SIZE;
				int absY = y + m_y * CHUNK_SIZE;
				int absZ = z + m_z * CHUNK_SIZE;

				// Front Face
				if(pWorld->getBlockID(absX, absY, absZ - 1) == BLOCK_AIR)
				{
					auto tex = block->texFront();
					float texL = (static_cast<float>(tex.first) * TEXTURE_STRIDE + TEXTURE_PADDING) / TEXTURE_ATLAS_SIZE;
					float texR = texL + TEXTURE_SIZE / TEXTURE_ATLAS_SIZE;
					float texT = (static_cast<float>(tex.second) * TEXTURE_STRIDE + TEXTURE_PADDING) / TEXTURE_ATLAS_SIZE;
					float texB = texT + TEXTURE_SIZE / TEXTURE_ATLAS_SIZE;

					vertices[vi].position = glm::vec3(static_cast<float>(absX), static_cast<float>(absY), static_cast<float>(absZ));
					vertices[vi].texCoord = glm::vec2(texR, texB);
					vertices[vi + 1].position = glm::vec3(static_cast<float>(absX), static_cast<float>(absY + 1), static_cast<float>(absZ));
					vertices[vi + 1].texCoord = glm::vec2(texR, texT);
					vertices[vi + 2].position = glm::vec3(static_cast<float>(absX + 1), static_cast<float>(absY), static_cast<float>(absZ));
					vertices[vi + 2].texCoord = glm::vec2(texL, texB);
					vertices[vi + 3].position = glm::vec3(static_cast<float>(absX + 1), static_cast<float>(absY + 1), static_cast<float>(absZ));
					vertices[vi + 3].texCoord = glm::vec2(texL, texT);

					indices[ii++] = vi;
					indices[ii++] = vi + 1;
					indices[ii++] = vi + 2;
					indices[ii++] = vi + 2;
					indices[ii++] = vi + 1;
					indices[ii++] = vi + 3;

					vi += 4;
				}

				// Back Face
				if(pWorld->getBlockID(absX, absY, absZ + 1) == BLOCK_AIR)
				{
					auto tex = block->texBack();
					float texL = (static_cast<float>(tex.first) * TEXTURE_STRIDE + TEXTURE_PADDING) / TEXTURE_ATLAS_SIZE;
					float texR = texL + TEXTURE_SIZE / TEXTURE_ATLAS_SIZE;
					float texT = (static_cast<float>(tex.second) * TEXTURE_STRIDE + TEXTURE_PADDING) / TEXTURE_ATLAS_SIZE;
					float texB = texT + TEXTURE_SIZE / TEXTURE_ATLAS_SIZE;

					vertices[vi].position = glm::vec3(static_cast<float>(absX), static_cast<float>(absY), static_cast<float>(absZ + 1));
					vertices[vi].texCoord = glm::vec2(texL, texB);
					vertices[vi + 1].position = glm::vec3(static_cast<float>(absX + 1), static_cast<float>(absY), static_cast<float>(absZ + 1));
					vertices[vi + 1].texCoord = glm::vec2(texR, texB);
					vertices[vi + 2].position = glm::vec3(static_cast<float>(absX), static_cast<float>(absY + 1), static_cast<float>(absZ + 1));
					vertices[vi + 2].texCoord = glm::vec2(texL, texT);
					vertices[vi + 3].position = glm::vec3(static_cast<float>(absX + 1), static_cast<float>(absY + 1), static_cast<float>(absZ + 1));
					vertices[vi + 3].texCoord = glm::vec2(texR, texT);

					indices[ii++] = vi;
					indices[ii++] = vi + 1;
					indices[ii++] = vi + 2;
					indices[ii++] = vi + 2;
					indices[ii++] = vi + 1;
					indices[ii++] = vi + 3;

					vi += 4;
				}

				// Left Face
				if(pWorld->getBlockID(absX + 1, absY, absZ) == BLOCK_AIR)
				{
					auto tex = block->texLeft();
					float texL = (static_cast<float>(tex.first) * TEXTURE_STRIDE + TEXTURE_PADDING) / TEXTURE_ATLAS_SIZE;
					float texR = texL + TEXTURE_SIZE / TEXTURE_ATLAS_SIZE;
					float texT = (static_cast<float>(tex.second) * TEXTURE_STRIDE + TEXTURE_PADDING) / TEXTURE_ATLAS_SIZE;
					float texB = texT + TEXTURE_SIZE / TEXTURE_ATLAS_SIZE;

					vertices[vi].position = glm::vec3(static_cast<float>(absX + 1), static_cast<float>(absY), static_cast<float>(absZ));
					vertices[vi].texCoord = glm::vec2(texR, texB);
					vertices[vi + 1].position = glm::vec3(static_cast<float>(absX + 1), static_cast<float>(absY + 1), static_cast<float>(absZ));
					vertices[vi + 1].texCoord = glm::vec2(texR, texT);
					vertices[vi + 2].position = glm::vec3(static_cast<float>(absX + 1), static_cast<float>(absY), static_cast<float>(absZ + 1));
					vertices[vi + 2].texCoord = glm::vec2(texL, texB);
					vertices[vi + 3].position = glm::vec3(static_cast<float>(absX + 1), static_cast<float>(absY + 1), static_cast<float>(absZ + 1));
					vertices[vi + 3].texCoord = glm::vec2(texL, texT);

					indices[ii++] = vi;
					indices[ii++] = vi + 1;
					indices[ii++] = vi + 2;
					indices[ii++] = vi + 2;
					indices[ii++] = vi + 1;
					indices[ii++] = vi + 3;

					vi += 4;
				}

				// Right Face
				if(pWorld->getBlockID(absX - 1, absY, absZ) == BLOCK_AIR)
				{
					auto tex = block->texRight();
					float texL = (static_cast<float>(tex.first) * TEXTURE_STRIDE + TEXTURE_PADDING) / TEXTURE_ATLAS_SIZE;
					float texR = texL + TEXTURE_SIZE / TEXTURE_ATLAS_SIZE;
					float texT = (static_cast<float>(tex.second) * TEXTURE_STRIDE + TEXTURE_PADDING) / TEXTURE_ATLAS_SIZE;
					float texB = texT + TEXTURE_SIZE / TEXTURE_ATLAS_SIZE;

					vertices[vi].position = glm::vec3(static_cast<float>(absX), static_cast<float>(absY), static_cast<float>(absZ));
					vertices[vi].texCoord = glm::vec2(texL, texB);
					vertices[vi + 1].position = glm::vec3(static_cast<float>(absX), static_cast<float>(absY), static_cast<float>(absZ + 1));
					vertices[vi + 1].texCoord = glm::vec2(texR, texB);
					vertices[vi + 2].position = glm::vec3(static_cast<float>(absX), static_cast<float>(absY + 1), static_cast<float>(absZ));
					vertices[vi + 2].texCoord = glm::vec2(texL, texT);
					vertices[vi + 3].position = glm::vec3(static_cast<float>(absX), static_cast<float>(absY + 1), static_cast<float>(absZ + 1));
					vertices[vi + 3].texCoord = glm::vec2(texR, texT);

					indices[ii++] = vi;
					indices[ii++] = vi + 1;
					indices[ii++] = vi + 2;
					indices[ii++] = vi + 2;
					indices[ii++] = vi + 1;
					indices[ii++] = vi + 3;

					vi += 4;
				}

				// Bottom Face
				if(pWorld->getBlockID(absX, absY - 1, absZ) == BLOCK_AIR)
				{
					auto tex = block->texBottom();
					float texL = (static_cast<float>(tex.first) * TEXTURE_STRIDE + TEXTURE_PADDING) / TEXTURE_ATLAS_SIZE;
					float texR = texL + TEXTURE_SIZE / TEXTURE_ATLAS_SIZE;
					float texT = (static_cast<float>(tex.second) * TEXTURE_STRIDE + TEXTURE_PADDING) / TEXTURE_ATLAS_SIZE;
					float texB = texT + TEXTURE_SIZE / TEXTURE_ATLAS_SIZE;

					vertices[vi].position = glm::vec3(static_cast<float>(absX), static_cast<float>(absY), static_cast<float>(absZ + 1));
					vertices[vi].texCoord = glm::vec2(texR, texB);
					vertices[vi + 1].position = glm::vec3(static_cast<float>(absX), static_cast<float>(absY), static_cast<float>(absZ));
					vertices[vi + 1].texCoord = glm::vec2(texR, texT);
					vertices[vi + 2].position = glm::vec3(static_cast<float>(absX + 1), static_cast<float>(absY), static_cast<float>(absZ + 1));
					vertices[vi + 2].texCoord = glm::vec2(texL, texB);
					vertices[vi + 3].position = glm::vec3(static_cast<float>(absX + 1), static_cast<float>(absY), static_cast<float>(absZ));
					vertices[vi + 3].texCoord = glm::vec2(texL, texT);

					indices[ii++] = vi;
					indices[ii++] = vi + 1;
					indices[ii++] = vi + 2;
					indices[ii++] = vi + 2;
					indices[ii++] = vi + 1;
					indices[ii++] = vi + 3;

					vi += 4;
				}

				// Top Face
				if(pWorld->getBlockID(absX, absY + 1, absZ) == BLOCK_AIR)
				{
					auto tex = block->texTop();
					float texL = (static_cast<float>(tex.first) * TEXTURE_STRIDE + TEXTURE_PADDING) / TEXTURE_ATLAS_SIZE;
					float texR = texL + TEXTURE_SIZE / TEXTURE_ATLAS_SIZE;
					float texT = (static_cast<float>(tex.second) * TEXTURE_STRIDE + TEXTURE_PADDING) / TEXTURE_ATLAS_SIZE;
					float texB = texT + TEXTURE_SIZE / TEXTURE_ATLAS_SIZE;

					vertices[vi].position = glm::vec3(static_cast<float>(absX), static_cast<float>(absY + 1), static_cast<float>(absZ + 1));
					vertices[vi].texCoord = glm::vec2(texL, texT);
					vertices[vi + 1].position = glm::vec3(static_cast<float>(absX + 1), static_cast<float>(absY + 1), static_cast<float>(absZ + 1));
					vertices[vi + 1].texCoord = glm::vec2(texR, texT);
					vertices[vi + 2].position = glm::vec3(static_cast<float>(absX), static_cast<float>(absY + 1), static_cast<float>(absZ));
					vertices[vi + 2].texCoord = glm::vec2(texL, texB);
					vertices[vi + 3].position = glm::vec3(static_cast<float>(absX + 1), static_cast<float>(absY + 1), static_cast<float>(absZ));
					vertices[vi + 3].texCoord = glm::vec2(texR, texB);

					indices[ii++] = vi;
					indices[ii++] = vi + 1;
					indices[ii++] = vi + 2;
					indices[ii++] = vi + 2;
					indices[ii++] = vi + 1;
					indices[ii++] = vi + 3;

					vi += 4;
				}
			}
		}
	}

	m_verticesBufCount = vi;
	m_indicesBufCount = ii;

	if((vi == 0) || (ii == 0))
	{
		delete m_verticesBuf;
		m_verticesBuf = nullptr;
		delete m_indicesBuf;
		m_indicesBuf = nullptr;
	}

	m_highPriorityUpdate = false;
}

void Chunk::draw()
{
	m_mesh.draw();
}

BlockID Chunk::block(int x, int y, int z)
{
	return m_blocks[getBlockIndex(x, y, z)];
}

void Chunk::addBlock(int x, int y, int z, BlockID type)
{
	m_blocks[getBlockIndex(x, y, z)] = type;

	m_dirty = m_highPriorityUpdate = true;
}

void Chunk::removeBlock(int x, int y, int z)
{
	m_blocks[getBlockIndex(x, y, z)] = BLOCK_AIR;

	m_dirty = m_highPriorityUpdate = true;
}

void Chunk::flushBuffer()
{
	if(m_indicesBuf && m_verticesBuf && (m_verticesBufCount != 0) && (m_indicesBufCount != 0))
	{
		m_mesh.setData(m_verticesBuf->data(), m_indicesBuf->data(), m_verticesBufCount, m_indicesBufCount);

		delete m_verticesBuf;
		m_verticesBuf = nullptr;
		delete m_indicesBuf;
		m_indicesBuf = nullptr;
		m_verticesBufCount = 0;
		m_indicesBufCount = 0;
	}
}