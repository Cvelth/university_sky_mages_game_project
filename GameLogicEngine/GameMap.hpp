#pragma once
#include <vector>
class AbstractBlock;
class GameCamera;
class RenderInfoStorage;
class RenderInfo;

enum DefaultMapFilling {
	All_Empty, All_Walls, Borders, Random,
	HorizontalRows, VerticalRows, Continious,
};

namespace Exceptions {
	class NonExistingCoordinateException {};
}

class GameMap {
	friend GameCamera;
private:
	size_t m_width;
	size_t m_height;
	std::vector<AbstractBlock*> m_blocks;
	std::vector<std::vector<AbstractBlock*>> m_cells;
protected:
	void fillEach(AbstractBlock* block);
	void borderFill(AbstractBlock* border, AbstractBlock* others);
	void randomFill(size_t number_of_types, AbstractBlock* types[]);
	void continiousFill(AbstractBlock* free, AbstractBlock* ceiling, AbstractBlock* floor,
						size_t max_step, size_t ceiling_start, size_t floor_start, size_t min_height);
	void horizontalRowsFill(AbstractBlock* odd, AbstractBlock* even);
	void verticalRowsFill(AbstractBlock* odd, AbstractBlock* even);

	void resize(size_t width, size_t height);
	void clear();

	void addNewBlock(AbstractBlock* block);
	inline void set(AbstractBlock* block, size_t width, size_t height) {
		if (width >= m_width || height >= m_height)
			throw Exceptions::NonExistingCoordinateException();
		addNewBlock(block);
		m_cells.at(width).at(height) = block;
	}

	bool isBorder(size_t w, size_t h) const;
public:
	GameMap(size_t width, size_t height, RenderInfoStorage* renderInfo, DefaultMapFilling mapFilling = DefaultMapFilling::All_Empty);
	~GameMap();

	inline size_t width() const { return m_width; }
	inline size_t height() const { return m_height; }
	inline size_t getSize() const { return m_width * m_height; }

	inline AbstractBlock* get(size_t w, size_t h) const {
		return m_cells.at(w).at(h);
	}
	inline AbstractBlock* operator()(size_t w, size_t h) const {
		return get(w, h);
	}
	inline std::vector<AbstractBlock*>& get_blocks_data() {
		return m_blocks;
	}
	float getSpeedMultiplier(size_t w, size_t h) const;
	RenderInfo const* getRenderInfo(size_t w, size_t h) const;
};