#include "Map.hpp"
#include "Block.hpp"

void Map::fillEach(Block* block) {
	addNewBlock(block);
	for (unsigned int i = 0; i < m_width; i++)
		for (unsigned int j = 0; j < m_height; j++)
			set(block, i, j);
}

void Map::borderFill(Block* border, Block* others) {
	addNewBlock(border);
	addNewBlock(others);
	for (unsigned int i = 0; i < m_width; i++)
		for (unsigned int j = 0; j < m_height; j++)
			if (isBorder(i, j))
				set(border, i, j);
			else
				set(others, i, j);
}

void Map::horizontalRowsFill(Block * odd, Block * even) {
	addNewBlock(odd);
	addNewBlock(even);
	for (unsigned int i = 0; i < m_width; i++)
		for (unsigned int j = 0; j < m_height; j += 2) {
			set(odd, i, j);
			set(even, i, j + 1);
		}
}

void Map::verticalRowsFill(Block* odd, Block* even) {
	addNewBlock(odd);
	addNewBlock(even);
	for (unsigned int j = 0; j < m_height; j++)
		for (unsigned int i = 0; i < m_width; i += 2) {
			set(odd, i, j);
			set(even, i + 1, j);
		}
}

void Map::addBorders(Block *border) {
	for (unsigned int i = 0; i < m_width; i++) {
		set(border, i, 0);
		set(border, i, m_height - 1);
	}
	for (unsigned int i = 0; i < m_height; i++) {
		set(border, 0, i);
		set(border, m_width - 2, i);
	}
}

void Map::resize(size_t width, size_t height) {
	m_width = width;
	m_height = height;
	m_cells.resize(width);
	for (unsigned int i = 0; i < width; i++)
		m_cells.at(i).resize(height);
}

void Map::clear() {
	for (auto it : m_blocks)
		delete it;
}

bool Map::isBorder(size_t w, size_t h) const {
	return w == 0 || w == m_width - 1 || h == 0 || h == m_height - 1;
}

Map::Map(size_t width, size_t height, DefaultMapFilling mapFilling) {
	resize(width, height);
	switch (mapFilling) {
		case DefaultMapFilling::All_Empty:
			fillEach(new EmptyBlock());
			break;
		case DefaultMapFilling::All_Walls:
			fillEach(new WallBlock());
			break;
		case DefaultMapFilling::Borders:
			borderFill(new WallBlock(), new EmptyBlock());
			break;
		case DefaultMapFilling::Random:	{
				Block* blocks[] = {new WallBlock(), new EmptyBlock()};
				randomFill(2, blocks);
			} break;
		case DefaultMapFilling::Continious: {
				Block* wall = new WallBlock();
				continiousFill(new EmptyBlock(), wall, wall, 3, height / 5, height - height / 5, 5);
			} break;
		case DefaultMapFilling::HorizontalRows:
			horizontalRowsFill(new WallBlock(), new EmptyBlock());
			break;
		case DefaultMapFilling::VerticalRows:
			verticalRowsFill(new WallBlock(), new EmptyBlock());
			break;
	}
	m_default_block = m_blocks.front();
}

Map::~Map() {
	clear();
}

float Map::getSpeedMultiplier(size_t w, size_t h) const {
	try {
		return get(w, h)->get();
	} catch (std::out_of_range& e) {
		return 0.f;
	}
}

RenderInfo const* Map::getRenderInfo(size_t w, size_t h) const {
	return get(w, h)->renderInfo();
}

#include <algorithm>
void Map::addNewBlock(Block * block) {
	if (std::find(m_blocks.begin(), m_blocks.end(), block) == m_blocks.end()) m_blocks.push_back(block);
}

#include <random>
void Map::randomFill(size_t number_of_types, Block* types[]) {
	std::random_device rd;
	std::mt19937 g(rd());
	std::uniform_int_distribution<size_t> d(0, number_of_types != 0 ? number_of_types - 1 : number_of_types);

	for (size_t i = 0; i < number_of_types; i++)
		addNewBlock(types[i]);
	for (unsigned int i = 0; i < m_width; i++)
		for (unsigned int j = 0; j < m_height; j++)
			set(types[d(g)], i, j);
}

void Map::continiousFill(Block* free, Block* ceiling, Block* floor, size_t max_step, size_t ceiling_start, size_t floor_start, size_t min_height) {
	std::random_device rd;
	std::mt19937 g(rd());
	std::uniform_int_distribution<int> d(-int(max_step), int(max_step));

	addNewBlock(free);
	addNewBlock(ceiling);
	addNewBlock(floor);

	size_t current_ceiling = ceiling_start;
	size_t current_floor = floor_start;
	for (unsigned int i = 0; i < m_width; i++) {
		unsigned int j = 0;
		for (; j < current_ceiling; j++)
			set(ceiling, i, j);
		for (; j < current_floor; j++)
			set(free, i, j);
		for (; j < m_height; j++)
			set(floor, i, j);

		current_ceiling += d(g);
		current_floor += d(g);
		int diff = int(current_floor) - int(current_ceiling);
		if (diff < 5) {
			size_t center = (current_floor + current_ceiling) / 2;
			current_floor = center + 3;
			current_ceiling = center - 2;
		}
		if (current_ceiling < min_height)
			current_ceiling = min_height;
		if (current_floor > m_height - min_height)
			current_floor = m_height - min_height;
	}

	addBorders(floor);
}