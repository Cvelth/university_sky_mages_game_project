#pragma once
#include <vector>
class AbstractBlock;

enum DefaultMapFilling {
	All_Empty, All_Walls, Borders, /*Random*/
	HorizontalRows, VerticalRows,
};

class GameMap {
private:
	size_t width, height;
	std::vector<std::vector<AbstractBlock*>> mapData;
protected:
	void fillEach(AbstractBlock* block);
	void borderFill(AbstractBlock* border, AbstractBlock* others);
	//void randomFill(size_t number_of_types, AbstractBlock** types);
	void horizontalRowsFill(AbstractBlock* odd, AbstractBlock* even);
	void verticalRowsFill(AbstractBlock* odd, AbstractBlock* even);

	void resize(size_t width, size_t height);
	void clear();

	inline void set(AbstractBlock* block, size_t width, size_t height) {
		mapData.at(width).at(height) = block;
	}

	bool isBorder(size_t w, size_t h) const;
public:
	GameMap(size_t width, size_t height, DefaultMapFilling mapFilling = DefaultMapFilling::All_Empty);
	~GameMap();

	inline AbstractBlock* get(size_t w, size_t h) const {
		return mapData.at(w).at(h);
	}
	inline AbstractBlock* operator()(size_t w, size_t h) const {
		return get(w, h);
	}
};

