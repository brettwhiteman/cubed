#ifndef CUBED_BLOCKS_H
#define CUBED_BLOCKS_H

#include <array>
#include "block_type.h"
#include <utility>

struct block_type_properties
{
	bool render;
	int hardness;
	std::pair<int, int> tex_front;
	std::pair<int, int> tex_back;
	std::pair<int, int> tex_left;
	std::pair<int, int> tex_right;
	std::pair<int, int> tex_bottom;
	std::pair<int, int> tex_top;
};

class blocks
{
public:
	blocks();

	const auto& get_properties(block_type type) const { return m_types[type]; }

private:
	void set_type(block_type type, bool render = false, int hardness = 0, std::pair<int, int> tex_front = {0, 0},
		std::pair<int, int> tex_back = {0, 0}, std::pair<int, int> tex_left = {0, 0}, std::pair<int, int> tex_right = {0, 0},
		std::pair<int, int> tex_bottom = {0, 0}, std::pair<int, int> tex_top = {0, 0})
	{
		m_types[type] = {render, hardness, tex_front, tex_back, tex_left, tex_right, tex_bottom, tex_top};
	}

	std::array<block_type_properties, NUM_BLOCK_TYPES> m_types;
};

#endif