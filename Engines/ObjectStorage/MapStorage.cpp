#include "MapStorage.hpp"
#include "../../Objects/Map/Block.hpp"

#include "../../Objects/Map/Map.hpp"
class MapSharedPointerEnabler : public Map { public: MapSharedPointerEnabler(size_t width, size_t height, std::shared_ptr<Block> default_block) : Map(width, height, default_block) {} };

void MapStorage::parse_file_type_info(std::string const& line) {
	if (line != " with a Map")
		throw Exceptions::FileParsingException("File seems to be corrupted");
}
#include <fstream>
#include "RenderInfoStorage.hpp"
void MapStorage::save(std::shared_ptr<Map> map, std::string const& filename, std::string const& path) {
	std::ofstream f;
	f.open(path + filename + MapFileExtention);
	if (!f)
		throw Exceptions::FileCannotBeOpennedException("It's impossible to write in the file. Some kind of system error.");

	f << generate_first_line(filename, MapFileExtention) << std::endl
		<< map_to_string(map);
}
#include <sstream>
std::pair<size_t, std::shared_ptr<Block>> read_block(std::string const& line) {
	std::istringstream s(line);
	size_t id;
	float temp;
	std::string render_info;
	s >> id >> temp >> render_info;
	return std::make_pair(id, std::make_shared<Block>(temp, RenderInfoStorage::getRenderInfo(render_info)));
}
std::pair<size_t, size_t> read_cell_data(std::string const& line) {
	std::istringstream s(line);
	size_t x, y;
	std::string string;
	s >> string >> x >> y;
	if (string != "CellData")
		throw Exceptions::FileParsingException("File seems to be corrupted");
	return std::make_pair(x, y);
}
std::shared_ptr<Map> MapStorage::load(std::string const& filename, std::string const& path, bool use) {
	std::ifstream f;
	f.open(path + filename + MapFileExtention);
	if (!f)
		throw Exceptions::FileCannotBeOpennedException("The file can't be openned for reading. Make sure it exists.");
	
	std::string string;
	std::getline(f, string); 
	parse_first_line(string, MapFileExtention);

	std::getline(f, string);
	if (string != "BlockData")
		throw Exceptions::FileParsingException("File seems to be corrupted");

	std::unordered_map<size_t, std::shared_ptr<Block>> ids;
	std::getline(f, string);
	while (f && string != "DefaultBlock") {
		ids.insert(read_block(string));
		std::getline(f, string);
	}

	std::getline(f, string);
	auto default_block = read_block(string);

	std::getline(f, string);
	auto size = read_cell_data(string);

	size_t temp;
	auto map = std::make_shared<MapSharedPointerEnabler>(size.first, size.second, default_block.second);
	for (size_t y = 0; y < size.second; y++)
		for (size_t x = 0; x < size.first; x++) {
			f >> temp;
			map->set(ids[temp], x, y);
		}
	return map;
}

std::string MapStorage::map_to_string(std::shared_ptr<Map> map) {
	std::ostringstream s;
	s << "BlockData\n";
	size_t current_id = 0;
	std::unordered_map<std::shared_ptr<Block>, size_t> ids;
	for (auto &it : map->get_blocks_data()) {
		ids.insert(std::make_pair(it, current_id));
		s << current_id++ << ' ' << it->get() << ' ' << RenderInfoStorage::getRenderInfo(it->renderInfo()) << '\n';
	}

	auto temp = std::find(map->get_blocks_data().begin(), map->get_blocks_data().end(), map->default_block());
	if (temp == map->get_blocks_data().end()) {
		ids.insert(std::make_pair(map->default_block(), current_id));
		s << "DefaultBlock\n" << current_id++ << ' ' << map->default_block()->get() << ' '
			<< RenderInfoStorage::getRenderInfo(map->default_block()->renderInfo()) << '\n';
	} else
		s << "DefaultBlock\n" << ids[*temp] << ' ' << (*temp)->get() << ' '
		<< RenderInfoStorage::getRenderInfo((*temp)->renderInfo()) << '\n';

	s << "CellData " << map->width() << ' ' << map->height() << "\n";
	for (size_t y = 0; y < map->height(); y++) {
		for (size_t x = 0; x < map->width(); x++)
			s << ids[map->get(x, y)] << ' ';
		s << '\n';
	}
	return s.str();
}
std::shared_ptr<Map> MapStorage::string_to_map(std::string const& data) {
	std::istringstream s(data);
	std::string string;

	std::getline(s, string);
	if (string != "BlockData")
		return nullptr;

	std::unordered_map<size_t, std::shared_ptr<Block>> ids;
	std::getline(s, string);
	while (s && string != "DefaultBlock") {
		ids.insert(read_block(string));
		std::getline(s, string);
	}

	std::getline(s, string);
	auto default_block = read_block(string);

	std::getline(s, string);
	auto size = read_cell_data(string);

	size_t temp;
	auto map = std::make_shared<MapSharedPointerEnabler>(size.first, size.second, default_block.second);
	for (size_t y = 0; y < size.second; y++)
		for (size_t x = 0; x < size.first; x++) {
			s >> temp;
			map->set(ids[temp], x, y);
		}
	return map;
}
