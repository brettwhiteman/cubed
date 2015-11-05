#ifndef BLOCKBEDROCK_H
#define BLOCKBEDROCK_H

#include "block.h"

class BlockBedrock : public Block
{
public:
	// Properties
	int hardness() { return 0; }

	// Rendering
	bool render() { return true; }

	// Texture atlas coordinates
	std::pair<int, int> texFront() { return std::make_pair(4, 0); }
	std::pair<int, int> texBack() { return std::make_pair(4, 0); }
	std::pair<int, int> texLeft() { return std::make_pair(4, 0); }
	std::pair<int, int> texRight() { return std::make_pair(4, 0); }
	std::pair<int, int> texBottom() { return std::make_pair(4, 0); }
	std::pair<int, int> texTop() { return std::make_pair(4, 0); }
};

#endif