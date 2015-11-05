#ifndef ITEMGRASS_H
#define ITEMGRASS_H

#include "item.h"

class ItemGrass : public Item
{
	inline std::pair<int, int> tex() { return std::make_pair(1, 0); }

	int maxStackSize() { return 64; }
};

#endif