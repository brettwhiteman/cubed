#ifndef BLOCKSTONE_H
#define BLOCKSTONE_H

#include "block_stone.h"

class BlockStone : public Block
{
public:
	// Properties
	int hardness() { return 2; }

	bool render() { return true; }

	// Texture atlas coordinates
	std::pair<int, int> texFront() { return std::make_pair(3, 0); }
	std::pair<int, int> texBack() { return std::make_pair(3, 0); }
	std::pair<int, int> texLeft() { return std::make_pair(3, 0); }
	std::pair<int, int> texRight() { return std::make_pair(3, 0); }
	std::pair<int, int> texBottom() { return std::make_pair(3, 0); }
	std::pair<int, int> texTop() { return std::make_pair(3, 0); }
};

#endif