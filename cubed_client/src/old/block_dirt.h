#ifndef BLOCKDIRT_H
#define BLOCKDIRT_H

#include "block.h"

class BlockDirt : public Block
{
public:
	// Properties
	int hardness() { return 8; }

	// Texture atlas coordinates
	std::pair<int, int> texFront() { return std::make_pair(2, 0); }
	std::pair<int, int> texBack() { return std::make_pair(2, 0); }
	std::pair<int, int> texLeft() { return std::make_pair(2, 0); }
	std::pair<int, int> texRight() { return std::make_pair(2, 0); }
	std::pair<int, int> texBottom() { return std::make_pair(2, 0); }
	std::pair<int, int> texTop() { return std::make_pair(2, 0); }
};

#endif