#include "Map.hpp"
#include "Block.hpp"

Map::Map(size_t width, size_t height, Block *default_block) : m_default_block(default_block) {
	m_cells.resize(width);
	for (size_t i = 0; i < width; i++)
		m_cells.at(i).resize(height);
}
Map::~Map() {
	for (auto it : m_blocks)
		delete it;
}
bool Map::is_in_borders(size_t w, size_t h) const {
	return w >= 0 && w < width() && h >= 0 && h < height();
}
Block* Map::get(size_t w, size_t h) const {
		if (is_in_borders(w, h))
			return m_cells.at(w).at(h);
		else
			return m_default_block;
}
float Map::getSpeedMultiplier(size_t w, size_t h) const {
	auto temp = get(w, h)->get();
	return temp;
}
RenderInfo const* Map::getRenderInfo(size_t w, size_t h) const {
	return get(w, h)->renderInfo();
}
bool Map::isBorder(size_t w, size_t h) const {
	return w == 0 || w == width() - 1 || h == 0 || h == height() - 1;
}
void Map::set(Block * block, size_t _width, size_t _height) {
	auto temp = height();
	if (_width < width() && _height < height()) {
		addNewBlock(block);
		m_cells.at(_width).at(_height) = block;
	} else
		throw Exceptions::NonExistingCellException();
}

#include <algorithm>
void Map::addNewBlock(Block *block) {
	if (std::find(m_blocks.begin(), m_blocks.end(), block) == m_blocks.end())
		m_blocks.push_back(block);
}