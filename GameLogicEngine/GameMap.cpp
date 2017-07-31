#include "GameMap.hpp"
#include "AbstractBlock.hpp"

void GameMap::fillEach(AbstractBlock* block) {
	m_blocks.insert(block);
	for (unsigned int i = 0; i < m_width; i++)
		for (unsigned int j = 0; j < m_height; j++)
			set(block, i, j);
}

void GameMap::borderFill(AbstractBlock* border, AbstractBlock* others) {
	m_blocks.insert(border);
	m_blocks.insert(others);
	for (unsigned int i = 0; i < m_width; i++)
		for (unsigned int j = 0; j < m_height; j++)
			if (isBorder(i, j))
				set(border, i, j);
			else
				set(others, i, j);
}

void GameMap::horizontalRowsFill(AbstractBlock * odd, AbstractBlock * even) {
	m_blocks.insert(odd);
	m_blocks.insert(even);
	for (unsigned int i = 0; i < m_width; i++)
		for (unsigned int j = 0; j < m_height; j += 2) {
			set(odd, i, j);
			set(even, i, j + 1);
		}
}

void GameMap::verticalRowsFill(AbstractBlock* odd, AbstractBlock* even) {
	m_blocks.insert(odd);
	m_blocks.insert(even);
	for (unsigned int j = 0; j < m_height; j++)
		for (unsigned int i = 0; i < m_width; i += 2) {
			set(odd, i, j);
			set(even, i + 1, j);
		}
}

void GameMap::resize(size_t width, size_t height) {
	m_width = width;
	m_height = height;
	m_cells.resize(width);
	for (unsigned int i = 0; i < width; i++)
		m_cells.at(i).resize(height);
}

void GameMap::clear() {
	for (auto it : m_blocks)
		delete it;
}

bool GameMap::isBorder(size_t w, size_t h) const {
	return w == 0 || w == m_width - 1 || h == 0 || h == m_height - 1;
}

GameMap::GameMap(size_t width, size_t height, RenderInfoStorage* renderInfo, DefaultMapFilling mapFilling) {
	resize(width, height);
	switch (mapFilling) {
		case DefaultMapFilling::All_Empty:
			fillEach(new EmptyBlock(renderInfo));
			break;
		case DefaultMapFilling::All_Walls:
			fillEach(new WallBlock(renderInfo));
			break;
		case DefaultMapFilling::Borders:
			borderFill(new WallBlock(renderInfo), new EmptyBlock(renderInfo));
			break;
		case DefaultMapFilling::Random:	{
				AbstractBlock* blocks[] = {new WallBlock(renderInfo), new EmptyBlock(renderInfo)};
				randomFill(2, blocks);
			} break;
		case DefaultMapFilling::Continious: {
				AbstractBlock* wall = new WallBlock(renderInfo);
				continiousFill(new EmptyBlock(renderInfo), wall, wall, 3, height / 5, height - height / 5, 5);
			} break;
		case DefaultMapFilling::HorizontalRows:
			horizontalRowsFill(new WallBlock(renderInfo), new EmptyBlock(renderInfo));
			break;
		case DefaultMapFilling::VerticalRows:
			verticalRowsFill(new WallBlock(renderInfo), new EmptyBlock(renderInfo));
			break;
	}
}

GameMap::~GameMap() {
	clear();
}

#include <random>
void GameMap::randomFill(size_t number_of_types, AbstractBlock* types[]) {
	std::random_device rd;
	std::mt19937 g(rd());
	std::uniform_int_distribution<size_t> d(0, number_of_types != 0 ? number_of_types - 1 : number_of_types);

	for (size_t i = 0; i < number_of_types; i++)
		m_blocks.insert(types[i]);
	for (unsigned int i = 0; i < m_width; i++)
		for (unsigned int j = 0; j < m_height; j++)
			set(types[d(g)], i, j);
}

void GameMap::continiousFill(AbstractBlock* free, AbstractBlock* ceiling, AbstractBlock* floor, size_t max_step, size_t ceiling_start, size_t floor_start, size_t min_height) {
	std::random_device rd;
	std::mt19937 g(rd());
	std::uniform_int_distribution<int> d(-int(max_step), max_step);

	m_blocks.insert(free);
	m_blocks.insert(ceiling);
	m_blocks.insert(floor);

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
}
