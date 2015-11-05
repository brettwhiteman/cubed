#include "chunk_updater.h"
#include <Windows.h>

#define LOW_PRIORITY_UPDATES 2

ChunkUpdater::ChunkUpdater() : m_running(false) { }

ChunkUpdater::~ChunkUpdater()
{
	m_running = false;
	if(m_thread.joinable())
		m_thread.join();
}

void ChunkUpdater::start()
{
	m_running = true;
	m_thread = std::thread(&ChunkUpdater::updateLoop, this);
}

void ChunkUpdater::add(std::shared_ptr<Chunk> chunk)
{
	if(chunk->m_highPriorityUpdate)
	{
		std::lock_guard<std::mutex> lock(m_lock);
		m_highUpdates.push_front(chunk);
		chunk->m_updatePending = true;
	}
	else
	{
		std::lock_guard<std::mutex> lock(m_lock);
		m_lowUpdates.push_front(chunk);
		chunk->m_updatePending = true;
	}
}

void ChunkUpdater::updateLoop()
{
	while(m_running)
	{
		std::shared_ptr<Chunk> chunk;
		bool noUpdates = true;

		{
			std::lock_guard<std::mutex> lock(m_lock);
			if(!m_highUpdates.empty())
			{
				chunk = m_highUpdates.back();
				m_highUpdates.pop_back();
				noUpdates = false;
			}
			else if(!m_lowUpdates.empty())
			{
				chunk = m_lowUpdates.back();
				m_lowUpdates.pop_back();
				noUpdates = false;
			}
		}
		
		if(chunk)
		{
			chunk->update();
			chunk->m_dirty = false;
			chunk->m_updatePending = false;
		}

		if(noUpdates)
			Sleep(5);
		else
			Sleep(1);
	}
}

std::string ChunkUpdater::getNumUpdatesPending()
{
	std::lock_guard<std::mutex> lock(m_lock);

	return std::to_string(m_highUpdates.size() + m_lowUpdates.size());
}