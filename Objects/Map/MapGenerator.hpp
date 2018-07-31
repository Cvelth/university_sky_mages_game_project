#pragma once
#include <vector>
class Block;
class Map;
class MapGenerator {
protected:
	static void addBorders(Map *map, Block *border);

	static Map* generate_single_block_filled_map(size_t width, size_t height, Block *block);
	static Map* generate_map_with_borders_only(size_t width, size_t height, Block *border, Block *empty);
	static Map* generate_random_map(size_t width, size_t height, std::vector<Block*> types);
	static Map* generate_continious_map(size_t width, size_t height, Block* ceiling, Block* floor, Block* free,
		size_t max_step, size_t ceiling_start, size_t floor_start, size_t min_height);
	static Map* generate_map_with_horizontal_rows_only(size_t width, size_t height, Block *odd, Block *even);
	static Map* generate_map_with_vertical_rows_only(size_t width, size_t height, Block *odd, Block *even);
public:
	static Map* generate_empty_map(size_t width, size_t height);
	static Map* generate_filled_map(size_t width, size_t height);
	static Map* generate_map_with_borders_only(size_t width, size_t height);
	static Map* generate_random_map(size_t width, size_t height);
	static Map* generate_continious_map(size_t width, size_t height);
	static Map* generate_map_with_horizontal_rows_only(size_t width, size_t height);
	static Map* generate_map_with_vertical_rows_only(size_t width, size_t height);
};