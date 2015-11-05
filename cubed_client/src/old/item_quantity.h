#ifndef ITEMQUANTITY_H
#define ITEMQUANTITY_H

#include "items.h"

class ItemQuantity
{
public:
	ItemQuantity() : m_itemID(ITEM_NOTHING), m_amount(0) { }

	ItemQuantity(ItemID id, int amount) : m_itemID(id), m_amount(amount >= 0 ? amount : 0) { }

	// Returns the amount left in other slot
	int operator+=(ItemQuantity& other)
	{
		if(m_itemID == other.m_itemID)
		{
			int transferAmount = Items::instance().get(m_itemID)->maxStackSize() - m_amount;

			if(transferAmount > other.m_amount)
			{
				transferAmount = other.m_amount;
			}

			m_amount += transferAmount;
			other.m_amount -= transferAmount;
		}

		return other.m_amount;
	}

	int operator-=(ItemQuantity& other)
	{
		if(m_itemID == other.m_itemID)
		{
			int transferAmount = m_amount;

			if(transferAmount > other.m_amount)
			{
				transferAmount = other.m_amount;
			}

			m_amount -= transferAmount;
			other.m_amount -= transferAmount;
		}

		return other.m_amount;
	}

	ItemID m_itemID;
	int m_amount;
};

#endif