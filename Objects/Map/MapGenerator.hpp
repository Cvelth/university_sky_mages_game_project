#pragma once
#include <memory>
#include <vector>
class Block;
class Map;
class MapGenerator {
protected:
	static void addBorders(std::shared_ptr<Map> map, std::shared_ptr<Block> border);

	static std::shared_ptr<Map> generate_single_block_filled_map(size_t width, size_t height, std::shared_ptr<Block> block);
	static std::shared_ptr<Map> generate_map_with_borders_only(size_t width, size_t height, std::shared_ptr<Block> border, std::shared_ptr<Block> empty);
	static std::shared_ptr<Map> generate_random_map(size_t width, size_t height, std::vector<std::shared_ptr<Block>> types);
	static std::shared_ptr<Map> generate_continious_map(size_t width, size_t height, std::shared_ptr<Block> ceiling, std::shared_ptr<Block> floor, std::shared_ptr<Block> free,
		size_t max_step, size_t ceiling_start, size_t floor_start, size_t min_height);
	static std::shared_ptr<Map> generate_map_with_horizontal_rows_only(size_t width, size_t height, std::shared_ptr<Block> odd, std::shared_ptr<Block> even);
	static std::shared_ptr<Map> generate_map_with_vertical_rows_only(size_t width, size_t height, std::shared_ptr<Block> odd, std::shared_ptr<Block> even);
public:
	static std::shared_ptr<Map> generate_empty_map(size_t width, size_t height);
	static std::shared_ptr<Map> generate_filled_map(size_t width, size_t height);
	static std::shared_ptr<Map> generate_map_with_borders_only(size_t width, size_t height);
	static std::shared_ptr<Map> generate_random_map(size_t width, size_t height);
	static std::shared_ptr<Map> generate_continious_map(size_t width, size_t height);
	static std::shared_ptr<Map> generate_map_with_horizontal_rows_only(size_t width, size_t height);
	static std::shared_ptr<Map> generate_map_with_vertical_rows_only(size_t width, size_t height);
};