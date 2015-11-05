#ifndef ITEMS_H
#define ITEMS_H

#define ITEM_TEXTURE_ATLAS_SIZE 512.0f
#define ITEM_TEXTURE_SIZE 16.0f
#define ITEM_TEXTURE_PADDING 2.0f
#define ITEM_TEXTURE_STRIDE 20.0f

#include "item.h"
#include <unordered_map>
#include <memory>
#include "texture.h"

enum ItemIDs
{
	ITEM_NOTHING,
	ITEM_GRASS,
	ITEM_STONE,
	ITEM_DIRT
};

typedef int ItemID;

class Items
{
public:
	Items();

	static Items& instance();

	template <typename T>
	void registerItemType(ItemID id);

	std::shared_ptr<Item> get(ItemID id);
	std::pair<float, float> getItemTextureCoords(ItemID item);

	Texture m_itemTextures;

private:
	std::unordered_map<ItemID, std::shared_ptr<Item>> m_itemTypes;
};

#endif