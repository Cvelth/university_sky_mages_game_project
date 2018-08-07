#pragma once
#include <string>
#include "FileLoader.hpp"
enum class ObjectType {
	Empty = 0, ClientSettings, ServerSettings,
	EnergyStorage, FlyEngine, Weapon
};
class Objects;
class ObjectStorage : public FileLoader {
private:
	Objects *m_objects;

	ObjectType m_current_object;
	size_t m_current_object_counter, m_current_object_number;
protected:
	void parse_line(std::string const& line);
	void parse_object_line(std::string const& line);
	void parse_first_line(std::string const& line);

	void initialize_object(ObjectType type, std::istream &s);
public:
	ObjectStorage(Objects* objects, std::string const& path = "/");
	void load(std::string const& path = "/");
};

#include "../../Shared/AbstractException.hpp"
DefineNewException(FileCannotBeOpennedException);
DefineNewException(FileParsingException);
DefineNewException(FileVersionException);