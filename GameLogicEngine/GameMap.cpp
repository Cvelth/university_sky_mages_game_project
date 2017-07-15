#include "GameMap.hpp"
#include "AbstractBlock.hpp"

void GameMap::fillEach(AbstractBlock * block) {
	for (unsigned int i = 0; i < width; i++)
		for (unsigned int j = 0; j < height; j++)
			set(block, i, j);
}

void GameMap::borderFill(AbstractBlock* border, AbstractBlock* others) {
	for (unsigned int i = 0; i < width; i++)
		for (unsigned int j = 0; j < height; j++)
			if (isBorder(i, j))
				set(border, i, j);
			else
				set(others, i, j);
}

void GameMap::horizontalRowsFill(AbstractBlock * odd, AbstractBlock * even) {
	for (unsigned int i = 0; i < width; i++)
		for (unsigned int j = 0; j < height; j += 2) {
			set(odd, i, j);
			set(even, i, j + 1);
		}
}

void GameMap::verticalRowsFill(AbstractBlock* odd, AbstractBlock* even) {
	for (unsigned int j = 0; j < height; j++)
		for (unsigned int i = 0; i < width; i += 2) {
			set(odd, i, j);
			set(even, i + 1, j);
		}
}

void GameMap::resize(size_t width, size_t height) {
	mapData.resize(width);
	for (unsigned int i = 0; i < width; i++)
		mapData.at(i).resize(height);
}

void GameMap::clear() {
	for (unsigned int i = 0; i < width; i++)
		for (unsigned int j = 0; j < height; j++) {
			auto t = get(i, j);
			if (t)
				delete t;
		}
}

bool GameMap::isBorder(size_t w, size_t h) const {
	return w == 0 || w == width - 1 || h == 0 || h == height - 1;
}

GameMap::GameMap(size_t width, size_t height, DefaultMapFilling mapFilling) : width(width), height(height) {
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
		case DefaultMapFilling::HorizontalRows:
			horizontalRowsFill(new WallBlock(), new EmptyBlock());
			break;
		case DefaultMapFilling::VerticalRows:
			verticalRowsFill(new WallBlock(), new EmptyBlock());
			break;
	}
}

GameMap::~GameMap() {
	clear();
}
