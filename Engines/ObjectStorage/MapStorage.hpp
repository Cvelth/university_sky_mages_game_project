#pragma once
#include "FileLoader.hpp"
#include "DefaultObjectStorageData.hpp"
#include "../../Objects/Map/Map.hpp"
class MapStorage : protected FileLoader {
protected:
	[[deprecated]] virtual void parse_line(std::string const& line) override {}
	virtual void parse_file_type_info(std::string const& line) override;
	virtual std::string generate_file_type_info() override { return "with a Map"; }
public:
	virtual void save(Map *map, std::string const& filename, std::string const& path);
	virtual Map* load(std::string const& filename, std::string const& path, bool use = true);
};