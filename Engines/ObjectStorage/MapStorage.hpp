#pragma once
#include "FileLoader.hpp"
#include "DefaultObjectStorageData.hpp"
#include "../../Objects/Map/Map.hpp"
class MapStorage : protected FileLoader {
protected:
	virtual void parse_line(std::string const& line) override { static_assert("Hasn't been implemented yet."); }
	virtual void parse_file_type_info(std::string const& line) override;
	virtual std::string generate_file_type_info() override { return "with a Map"; }
public:
	virtual void save(Map *map, std::string const& filename, std::string const& path);
	virtual Map* load_map(std::string const& filename, std::string const& path) { static_assert("Hasn't been implemented yet."); return nullptr; };
};