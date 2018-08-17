#pragma once
#include <vector>
#include <memory>
class Block;
class Camera;
class RenderInfo;
class MapGenerator;
class MapStorage;

class Map {
	friend MapGenerator;
	friend MapStorage;
private:
	std::vector<Block*> m_blocks;
	std::vector<std::vector<Block*>> m_cells;
	Block* m_default_block;
protected:
	Map(size_t width, size_t height, Block *default_block);
	void set(Block* block, size_t _width, size_t _height);
	void addNewBlock(Block *block);
	bool isBorder(size_t w, size_t h) const;
public:
	~Map();

	inline size_t width() const { return m_cells.size(); }
	inline size_t height() const { return m_cells.front().size(); }
	inline size_t getSize() const { return width() * height(); }

	bool is_in_borders(size_t w, size_t h) const;
	inline Block* operator()(size_t w, size_t h) const {
		return get(w, h);
	}
	Block* get(size_t w, size_t h) const;
	float getSpeedMultiplier(size_t w, size_t h) const;
	std::shared_ptr<RenderInfo> const getRenderInfo(size_t w, size_t h) const;
	inline std::vector<Block*>& get_blocks_data() {
		return m_blocks;
	}
	inline void changeDefaultBlock(Block *new_default) {
		m_default_block = new_default;
	}
	inline Block* default_block() const { return m_default_block; }
};

#include "../../Shared/AbstractException.hpp"
DefineNewException(NonExistingCellException)