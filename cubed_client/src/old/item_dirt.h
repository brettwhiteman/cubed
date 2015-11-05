#ifndef ITEMDIRT_H
#define ITEMDIRT_H

#include "item.h"

class ItemDirt : public Item
{
	inline std::pair<int, int> tex() { return std::make_pair(2, 0); }

	int maxStackSize() { return 64; }
};

#endif