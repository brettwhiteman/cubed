#ifndef CHUNKUPDATER_H
#define CHUNKUPDATER_H

#include <list>
#include "chunk.h"
#include <atomic>
#include <thread>
#include <mutex>

class ChunkUpdater
{
public:
	ChunkUpdater();
	~ChunkUpdater();

	void start();

	void add(std::shared_ptr<Chunk> chunk);

	void updateLoop();

	std::string getNumUpdatesPending();

private:
	std::list<std::shared_ptr<Chunk>> m_lowUpdates;
	std::list<std::shared_ptr<Chunk>> m_highUpdates;
	std::mutex m_lock;
	std::atomic<bool> m_running;
	std::thread m_thread;
};

#endif