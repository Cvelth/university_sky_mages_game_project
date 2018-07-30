#pragma once
#include <vector>
class Block;
class Camera;
class RenderInfo;

enum DefaultMapFilling {
	All_Empty, All_Walls, Borders, Random,
	HorizontalRows, VerticalRows, Continious,
};

namespace Exceptions {
	class NonExistingCoordinateException {};
}

class Map {
	friend Camera;
private:
	size_t m_width;
	size_t m_height;
	Block* m_default_block;

	std::vector<Block*> m_blocks;
	std::vector<std::vector<Block*>> m_cells;
protected:
	void fillEach(Block* block);
	void borderFill(Block* border, Block* others);
	void randomFill(size_t number_of_types, Block* types[]);
	void continiousFill(Block* free, Block* ceiling, Block* floor,
						size_t max_step, size_t ceiling_start, size_t floor_start, size_t min_height);
	void horizontalRowsFill(Block* odd, Block* even);
	void verticalRowsFill(Block* odd, Block* even);

	void addBorders(Block* border);

	void resize(size_t width, size_t height);
	void clear();

	void addNewBlock(Block* block);
	inline void set(Block* block, size_t width, size_t height) {
		if (width >= m_width || height >= m_height)
			throw Exceptions::NonExistingCoordinateException();
		addNewBlock(block);
		m_cells.at(width).at(height) = block;
	}

	bool isBorder(size_t w, size_t h) const;
public:
	Map(size_t width, size_t height, DefaultMapFilling mapFilling = DefaultMapFilling::All_Empty);
	~Map();

	inline size_t width() const { return m_width; }
	inline size_t height() const { return m_height; }
	inline size_t getSize() const { return m_width * m_height; }

	inline Block* get(size_t w, size_t h) const {
		if (w >= 0 && w < width() && h >= 0 && h < height())
			return m_cells.at(w).at(h);
		else
			return m_default_block;
	}
	inline Block* operator()(size_t w, size_t h) const {
		return get(w, h);
	}
	inline std::vector<Block*>& get_blocks_data() {
		return m_blocks;
	}
	float getSpeedMultiplier(size_t w, size_t h) const;
	RenderInfo const* getRenderInfo(size_t w, size_t h) const;
};