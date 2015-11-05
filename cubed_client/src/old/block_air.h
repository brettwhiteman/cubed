#ifndef BLOCKAIR_H
#define BLOCKAIR_H

#include "block.h"

class BlockAir : public Block
{
public:
	// Properties
	int hardness() { return 0; }

	// Rendering
	bool render() { return false; }

	// Texture atlas coordinates
	std::pair<int, int> texFront() { return std::make_pair(0, 0); }
	std::pair<int, int> texBack() { return std::make_pair(0, 0); }
	std::pair<int, int> texLeft() { return std::make_pair(0, 0); }
	std::pair<int, int> texRight() { return std::make_pair(0, 0); }
	std::pair<int, int> texBottom() { return std::make_pair(0, 0); }
	std::pair<int, int> texTop() { return std::make_pair(0, 0); }
};

#endif