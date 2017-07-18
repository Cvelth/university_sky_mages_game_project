#pragma once
#include <vector>
class AbstractBlock;
class GameCamera;

enum DefaultMapFilling {
	All_Empty, All_Walls, Borders, /*Random*/
	HorizontalRows, VerticalRows,
};

namespace Exceptions {
	class NonExistingCoordinateException {};
}

class GameMap {
	friend GameCamera;
private:
	size_t m_width;
	size_t m_height;
	std::vector<std::vector<AbstractBlock*>> m_mapData;
protected:
	void fillEach(AbstractBlock* block);
	void borderFill(AbstractBlock* border, AbstractBlock* others);
	//void randomFill(size_t number_of_types, AbstractBlock** types);
	void horizontalRowsFill(AbstractBlock* odd, AbstractBlock* even);
	void verticalRowsFill(AbstractBlock* odd, AbstractBlock* even);

	void resize(size_t width, size_t height);
	void clear();

	inline void set(AbstractBlock* block, size_t width, size_t height) {
		if (width >= m_width || height >= m_height)
			throw Exceptions::NonExistingCoordinateException();
		m_mapData.at(width).at(height) = block;
	}

	bool isBorder(size_t w, size_t h) const;
public:
	GameMap(size_t width, size_t height, DefaultMapFilling mapFilling = DefaultMapFilling::All_Empty);
	~GameMap();

	inline AbstractBlock* get(size_t w, size_t h) const {
		return m_mapData.at(w).at(h);
	}
	inline AbstractBlock* operator()(size_t w, size_t h) const {
		return get(w, h);
	}
};

