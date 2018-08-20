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
	virtual void save(std::shared_ptr<Map> map, std::string const& filename, std::string const& path);
	virtual std::shared_ptr<Map> load(std::string const& filename, std::string const& path, bool use = true);

	static std::string map_to_string(std::shared_ptr<Map> map);
	static std::shared_ptr<Map> string_to_map(std::string const& data);
};