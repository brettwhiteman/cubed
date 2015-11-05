#include "inventory.h"

Inventory::Inventory()
{
	m_handSlot.m_itemID = ITEM_GRASS;
	m_slots[1].m_itemID = ITEM_STONE;
	InputManager::instance().addKeyReleaseHandler(SDL_SCANCODE_E, std::bind(&Inventory::toggle, this));
}

void Inventory::onSlotLeftClick(ItemQuantity& slot)
{
	ItemQuantity temp = slot;
	slot = m_handSlot;
	m_handSlot = temp;
}

void Inventory::onSlotRightClick(ItemQuantity& slot)
{
	onSlotLeftClick(slot);
}

void Inventory::toggle()
{
	if(m_open)
	{
		m_open = false;
		InputManager::instance().m_state = STATE_NORMAL;
		InputManager::instance().centreMouse();
	}
	else
	{
		m_open = true;
		InputManager::instance().m_state = STATE_INVENTORY;
	}
}