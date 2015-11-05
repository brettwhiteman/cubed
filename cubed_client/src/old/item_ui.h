#ifndef _ITEMUI_H_
#define _ITEMUI_H_

#include <array>
#include "item_quantity.h"
#include "mesh_pti.h"

template <int T_CX, int T_CY, int T_G_W, int T_G_H, int T_G_PX, int T_G_PY>
class ItemUI
{
public:
	ItemUI() : m_mesh(false), m_open(false)
	{
		InputManager::instance().addLeftClickHandler(std::bind(&ItemUI::handleLeftClick, this));
		InputManager::instance().addRightClickHandler(std::bind(&ItemUI::handleLeftClick, this));
	}

	void updateMesh(float windowMidX, float windowMidY)
	{
		std::vector<Vertex_PT> vertices;
		std::vector<unsigned short> indices;

		float sx = windowMidX - (static_cast<float>(T_CX) / 2.0f) * (T_G_W + T_G_PX);
		float sy = windowMidY - (static_cast<float>(T_CY) / 2.0f) * (T_G_H + T_G_PY);
		float px = sx;
		float py = sy;

		unsigned short index = 0;

		float tw = ITEM_TEXTURE_SIZE / ITEM_TEXTURE_ATLAS_SIZE;

		for(int y = 0; y < T_CY; ++y)
		{
			for(int x = 0; x < T_CX; ++x)
			{
				int i = T_CX * y + x;
				const std::pair<float, float> tex = Items::instance().getItemTextureCoords(m_slots[i].m_itemID);

				vertices.push_back(Vertex_PT(glm::vec3(px, py, 0.0f), glm::vec2(tex.first, tex.second + tw)));
				vertices.push_back(Vertex_PT(glm::vec3(px, py + T_G_H, 0.0f), glm::vec2(tex.first, tex.second)));
				vertices.push_back(Vertex_PT(glm::vec3(px + T_G_W, py, 0.0f), glm::vec2(tex.first + tw, tex.second + tw)));
				vertices.push_back(Vertex_PT(glm::vec3(px + T_G_W, py + T_G_H, 0.0f), glm::vec2(tex.first + tw, tex.second)));

				indices.push_back(index);
				indices.push_back(index + 2);
				indices.push_back(index + 1);
				indices.push_back(index + 2);
				indices.push_back(index + 3);
				indices.push_back(index + 1);

				index += 4;

				if(x == T_CX - 1)
				{
					px = sx;
					py += T_G_H + T_G_PY;
				}
				else
				{
					px += T_G_W + T_G_PX;
				}
			}
		}

		if(m_handSlot.m_itemID != ITEM_NOTHING)
		{
			auto mousePos = InputManager::instance().getMousePosition();
			float x = static_cast<float>(mousePos.first) - T_G_W / 2.0f;
			float y = Globals::game->m_renderingEngine.getWindowHeight() -(static_cast<float>(mousePos.second) + T_G_H / 2.0f);

			const std::pair<float, float> tex = Items::instance().getItemTextureCoords(m_handSlot.m_itemID);

			vertices.push_back(Vertex_PT(glm::vec3(x, y, 0.0f), glm::vec2(tex.first, tex.second + tw)));
			vertices.push_back(Vertex_PT(glm::vec3(x, y + T_G_H, 0.0f), glm::vec2(tex.first, tex.second)));
			vertices.push_back(Vertex_PT(glm::vec3(x + T_G_W, y, 0.0f), glm::vec2(tex.first + tw, tex.second + tw)));
			vertices.push_back(Vertex_PT(glm::vec3(x + T_G_W, y + T_G_H, 0.0f), glm::vec2(tex.first + tw, tex.second)));

			indices.push_back(index);
			indices.push_back(index + 2);
			indices.push_back(index + 1);
			indices.push_back(index + 2);
			indices.push_back(index + 3);
			indices.push_back(index + 1);

			index += 4;
		}

		m_mesh.setData(vertices.data(), indices.data(), vertices.size(), indices.size());
	}

	void draw() { updateMesh(); m_mesh.draw(); }
	bool render() { return m_open; }

	int getCursorGridIndex()
	{
		auto mousePos = InputManager::instance().getMousePosition();

		float sx = static_cast<float>(Globals::game->m_windowMidX);
		float sy = static_cast<float>(Globals::game->m_windowMidY);
		sx -= (static_cast<float>(T_CX) / 2.0f) * (T_G_W + T_G_PX);
		sy -= (static_cast<float>(T_CY) / 2.0f) * (T_G_H + T_G_PY);

		float mx = static_cast<float>(mousePos.first) - sx;
		float my = static_cast<float>(Globals::game->m_renderingEngine.getWindowHeight() - mousePos.second) - sy;
		int ix = static_cast<int>((mx < 0.0f ? (mx - (T_G_W + T_G_PX)) : mx) / (T_G_W + T_G_PX));
		int iy = static_cast<int>((my < 0.0f ? (my - (T_G_H + T_G_PY)) : my) / (T_G_H + T_G_PY));

		if(ix < 0 || ix >= T_CX || iy < 0 || iy >= T_CY || mx >(ix + 1) * (T_G_W + T_G_PX) - T_G_PX ||
			my >(iy + 1) * (T_G_H + T_G_PY) - T_G_PY)
			return -1;

		return T_CX * iy + ix;
	}

	void handleLeftClick()
	{
		if(m_open)
		{
			int i = getCursorGridIndex();

			if(i >= 0)
				onSlotLeftClick(m_slots[i]);
		}
	}

	void handleRightClick()
	{
		if(m_open)
		{
			int i = getCursorGridIndex();

			if(i >= 0)
				onSlotRightClick(m_slots[i]);
		}
	}

	virtual void onSlotLeftClick(ItemQuantity& slot) = 0;
	virtual void onSlotRightClick(ItemQuantity& slot) = 0;

protected:
	std::array<ItemQuantity, T_CX * T_CY> m_slots;
	ItemQuantity m_handSlot;
	bool m_open;

private:
	Mesh_PTI m_mesh;
};

#endif