#ifndef ITEMNOTHING_H
#define ITEMNOTHING_H

#include "item.h"

class ItemNothing : public Item
{
	inline std::pair<int, int> tex() { return std::make_pair(4, 0); }

	int maxStackSize() { return 0; }
};

#endif