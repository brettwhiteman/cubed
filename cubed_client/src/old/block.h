#ifndef BLOCK_H
#define BLOCK_H

#include <utility>

class Block
{
public:
	virtual ~Block() { };

	// Properties
	virtual int hardness() = 0;

	// Rendering properties
	virtual bool render() { return true; }

	// Texture atlas coordinates
	virtual std::pair<int, int> texFront() = 0;
	virtual std::pair<int, int> texBack() = 0;
	virtual std::pair<int, int> texLeft() = 0;
	virtual std::pair<int, int> texRight() = 0;
	virtual std::pair<int, int> texBottom() = 0;
	virtual std::pair<int, int> texTop() = 0;

	// functions
	virtual void onBreak(int x, int y, int z) { }
	virtual void onPlace(int x, int y, int z) { }
	// returns true if activatable (so a block won't be placed on right click)
	virtual bool activate(int x, int y, int z) { return false; }
};

#endif