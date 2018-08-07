#include "MapStorage.hpp"
#include "../../Objects/Map/Map.hpp"
#include "../../Objects/Map/Block.hpp"
void MapStorage::parse_file_type_info(std::string const& line) {
	if (line != "with a Map")
		throw Exceptions::FileParsingException("File seems to be corrupted");
}
#include <fstream>
#include "RenderInfoStorage.hpp"
void MapStorage::save(Map *map, std::string const& filename, std::string const& path) {
	std::ofstream f;
	f.open(path + filename + MapFileExtention);
	if (!f)
		throw Exceptions::FileCannotBeOpennedException("It's impossible to write in file. Some kind of system error");

	f << generate_first_line(filename, MapFileExtention) << std::endl;

	f << "BlockData\n";
	size_t current_id = 0;
	std::unordered_map<Block*, size_t> ids;
	for (auto &it : map->get_blocks_data()) {
		ids.insert(std::make_pair(it, current_id));
		f << current_id++ << ' ' << it->get() << ' ' << RenderInfoStorage::getRenderInfo(it->renderInfo()) << '\n';
	}

	auto temp = std::find(map->get_blocks_data().begin(), map->get_blocks_data().end(), map->default_block());
	if (temp == map->get_blocks_data().end()) {
		ids.insert(std::make_pair(map->default_block(), current_id));
		f << "DefaultBlock: " << current_id++ << ' ' << map->default_block()->get() << ' '
			<< RenderInfoStorage::getRenderInfo(map->default_block()->renderInfo()) << '\n';
	} else
		f << "DefaultBlock: " << ids[*temp] << ' ' << (*temp)->get() << ' '
			<< RenderInfoStorage::getRenderInfo((*temp)->renderInfo()) << '\n';

	f << "CellData " << map->width() << ' ' << map->height() << "\n";
	for (size_t y = 0; y < map->height(); y++) {
		for (size_t x = 0; x < map->width(); x++)
			f << ids[map->get(x, y)] << ' ';
		f << '\n';
	}
}