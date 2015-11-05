#ifndef INVENTORY_H
#define INVENTORY_H

#include "item_ui.h"

class Inventory : public ItemUI<9, 4, 40, 40, 5, 5>
{
public:
	Inventory();

	void onSlotLeftClick(ItemQuantity& slot);
	void onSlotRightClick(ItemQuantity& slot);
	void toggle();
};

#endif