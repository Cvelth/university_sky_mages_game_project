#pragma once
#include <memory>
#include <string>
#include "FileLoader.hpp"
#include "DefaultObjectStorageData.hpp"
enum class ObjectType {
	Empty = 0, ClientSettings, ServerSettings,
	EnergyStorage, FlyEngine, Weapon, ShieldGenerator, Trinket, Upgrade
};
class Objects;
class ObjectStorage : protected FileLoader {
private:
	std::shared_ptr<Objects> m_objects;

	ObjectType m_current_object;
	size_t m_current_object_counter, m_current_object_number;
protected:
	virtual void parse_file_type_info(std::string const& line) override;
	virtual void parse_line(std::string const& line) override;
	virtual void parse_object_line(std::string const& line);

	virtual std::string generate_file_type_info() override;

	void initialize_object(ObjectType type, std::istream &s);
public:
	ObjectStorage(std::shared_ptr<Objects> objects, std::string const& path = "/");
	void load(std::string const& path = "/") { FileLoader::load(path, ObjectStorageFileExtention); }
	virtual void save(std::string const& filename, std::string const& path);
};