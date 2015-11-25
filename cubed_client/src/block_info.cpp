#include "block_info.h"

BlockInfo::BlockInfo()
{
	set_type(BLOCK_AIR);
	set_type(BLOCK_GRASS, true, 1500, {1, 0}, {1, 0}, {1, 0}, {1, 0}, {2, 0}, {0, 0});
	set_type(BLOCK_STONE, true, 4000, {3, 0}, {3, 0}, {3, 0}, {3, 0}, {3, 0}, {3, 0});
	set_type(BLOCK_DIRT, true, 1200, {2, 0}, {2, 0}, {2, 0}, {2, 0}, {2, 0}, {2, 0});
}