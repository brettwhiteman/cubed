#ifndef BLOCKGRASS_H
#define BLOCKGRASS_H

#include "block.h"
#include "globals.h"
#include "game.h"

class BlockGrass : public Block
{
public:
	// Properties
	int hardness() { return 10; }

	// Texture atlas coordinates
	std::pair<int, int> texFront() { return std::make_pair(1, 0); }
	std::pair<int, int> texBack() { return std::make_pair(1, 0); }
	std::pair<int, int> texLeft() { return std::make_pair(1, 0); }
	std::pair<int, int> texRight() { return std::make_pair(1, 0); }
	std::pair<int, int> texBottom() { return std::make_pair(2, 0); }
	std::pair<int, int> texTop() { return std::make_pair(0, 0); }

	void onPlace(int x, int y, int z)
	{
		/*for(int _x = x - 2; _x <= x + 2; ++_x)
		{
			for(int _y = y - 2; _y <= y + 2; ++_y)
			{
				for(int _z = z - 2; _z <= z + 2; ++_z)
				{
					Globals::game->m_world.removeBlock(_x, _y, _z);
				}
			}
		}*/
	}
};

#endif