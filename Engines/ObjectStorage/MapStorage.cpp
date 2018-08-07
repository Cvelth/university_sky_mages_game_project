#include "MapStorage.hpp"
void MapStorage::parse_file_type_info(std::string const& line) {
	if (line != "with a Map")
		throw Exceptions::FileParsingException("File seems to be corrupted");
}
#include <fstream>
void MapStorage::save(Map *map, std::string const& filename, std::string const& path) {
	std::ofstream f;
	f.open(path + filename + MapFileExtention);
	if (!f)
		throw Exceptions::FileCannotBeOpennedException("It's impossible to write in file. Some kind of system error");

	f << generate_first_line(filename, MapFileExtention) << std::endl;

	f << "BlockData\n";
	for (auto &it : map->get_blocks_data())
		f << "Block here\n";

	f << "CellData\n";

}