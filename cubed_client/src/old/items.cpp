#include "items.h"
#include "item_nothing.h"
#include "item_grass.h"
#include "item_dirt.h"
#include "item_stone.h"

Items::Items() : m_itemTextures("res\\items.png")
{
	registerItemType<ItemNothing>(ITEM_NOTHING);
	registerItemType<ItemGrass>(ITEM_GRASS);
	registerItemType<ItemDirt>(ITEM_DIRT);
	registerItemType<ItemStone>(ITEM_STONE);
}

Items& Items::instance()
{
	static Items items;

	return items;
}

template <typename T>
void Items::registerItemType(ItemID id)
{
	auto it = m_itemTypes.find(id);

	if(it != m_itemTypes.end())
	{
		return;
	}

	m_itemTypes.emplace(id, std::shared_ptr<Item>(new T()));
}

std::shared_ptr<Item> Items::get(ItemID id)
{
	auto it = m_itemTypes.find(id);

	if(it == m_itemTypes.end())
	{
		return nullptr;
	}

	return it->second;
}

std::pair<float, float> Items::getItemTextureCoords(ItemID item)
{
	std::pair<int, int> coords = get(item)->tex();

	float tx = static_cast<float>(coords.first);
	float ty = static_cast<float>(coords.second);

	return std::make_pair((tx * ITEM_TEXTURE_STRIDE + ITEM_TEXTURE_PADDING) / ITEM_TEXTURE_ATLAS_SIZE, (ty * ITEM_TEXTURE_STRIDE + ITEM_TEXTURE_PADDING) / ITEM_TEXTURE_ATLAS_SIZE);
}