#ifndef ITEM_H
#define ITEM_H

#include <utility>

class Item
{
public:
	virtual ~Item() { }

	virtual std::pair<int, int> tex() = 0;

	virtual int maxStackSize() = 0;
};

#endif