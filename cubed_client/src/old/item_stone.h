#ifndef ITEMSTONE_H
#define ITEMSTONE_H

#include "item.h"

class ItemStone : public Item
{
	inline std::pair<int, int> tex() { return std::make_pair(3, 0); }

	int maxStackSize() { return 64; }
};

#endif