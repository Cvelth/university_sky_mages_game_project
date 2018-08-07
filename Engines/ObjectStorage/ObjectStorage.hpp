#pragma once
#include <string>
#include "FileLoader.hpp"
enum class ObjectType {
	Empty = 0, ClientSettings, ServerSettings,
	EnergyStorage, FlyEngine, Weapon
};
class Objects;
class ObjectStorage : protected FileLoader {
private:
	Objects *m_objects;

	ObjectType m_current_object;
	size_t m_current_object_counter, m_current_object_number;
protected:
	virtual void parse_file_type_info(std::string const& line) override;
	virtual void parse_line(std::string const& line) override;
	virtual void parse_object_line(std::string const& line);

	void initialize_object(ObjectType type, std::istream &s);
public:
	ObjectStorage(Objects* objects, std::string const& path = "/");
	void load(std::string const& path = "/") { FileLoader::load(path, ".stg"); }
};