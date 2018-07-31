#include "ObjectStorage.hpp"
#include "DefaultObjectStorageData.hpp"
#include "Objects.hpp"

ObjectStorage::ObjectStorage(Objects *objects, std::string const& path)
	: m_objects(objects), m_current_object(ObjectType::Empty),
	m_current_object_counter(0), m_current_object_number(0) { load(path); }

ObjectType _switch(std::string const& type) {
	if (type == "ClientSettings")
		return ObjectType::ClientSettings;
	else if (type == "ServerSettings")
		return ObjectType::ServerSettings;
	else
		return ObjectType::Empty;
}

#include <string>
#include <sstream>
void ObjectStorage::parse_line(std::string const& line) {
	std::istringstream s(line);
	std::string string;
	s >> string;
	if (string == "Object") {
		char placeholder;
		size_t number;
		s >> number >> placeholder >> string;
		if (number == ++m_current_object_counter && number <= m_current_object_number)
			m_current_object = _switch(string);
		else
			throw Exceptions::FileParsingException("File seems to be corrupted");
	} else if (m_current_object != ObjectType::Empty) {
		return parse_object_line(line);
	} else
		throw Exceptions::FileParsingException("File seems to be corrupted");
}
void ObjectStorage::parse_first_line(std::string const& line) {
	std::istringstream s(line);
	std::string string;
	s >> string;
	if (string.size() < 4 || string.substr(string.size() - 4) != ObjectStorageFileExtention)
		throw Exceptions::FileParsingException("File seems to be corrupted");
	s >> string >> string >> string;
	if (string != "ObjectStorage")
		throw Exceptions::FileParsingException("File seems to be corrupted");

	char placeholder;
	size_t major, minor, patch, build;
	s >> placeholder >> major >> placeholder >> minor;
	if (major != Object_Storage_Syntax_Major_Version || minor != Object_Storage_Syntax_Minor_Version)
		throw Exceptions::FileVersionException("File storage version is different from the expected one.");

	s >> string >> string >> string;
	if (string != Program_Name)
		throw Exceptions::FileParsingException("File seems to be corrupted");

	s >> placeholder >> major >> placeholder >> minor >> placeholder >> patch >> 
		placeholder >> build >> placeholder >> string;
	if (major != Program_Major_Version || minor != Program_Minor_Version || patch != Program_Patch_Version
		|| build != Program_Build_Version || string != Program_Version_Suffix) 
	{
		throw Exceptions::FileVersionException("Program version is different from the expected one.");
	}

	s >> string >> major >> string;
	if (string != "objects")
		throw Exceptions::FileParsingException("File seems to be corrupted");

	m_current_object_number = major;
}

#include <list>
std::list<std::string> split_path(std::string const& path, std::string const& separator = ";") {
	std::list<std::string> ret;
	size_t position, last_position = 0u, length = path.length();
	while (last_position < length + 1) {
		position = path.find_first_of(separator, last_position);
		if (position == std::string::npos) position = length;
		ret.push_back(std::string{path.data() + last_position, position - last_position});
		last_position = position + 1;
	}
	return ret;
}

#include <filesystem>
#include <fstream>
void ObjectStorage::load(std::string const& path_string) {
	for (auto path : split_path(path_string))
		for (auto file : std::filesystem::recursive_directory_iterator(std::filesystem::current_path().generic_string() + path))
			if (auto temp = file.path().generic_string(); temp.substr(temp.size() - 4) == ObjectStorageFileExtention) {
				std::ifstream f;
				f.open(temp);
				if (!f.is_open())
					throw Exceptions::FileCannotBeOpennedException("Filesystem parsing failure detected");

				bool first_line_parsed = false;
				std::string temp_line;
				while (std::getline(f, temp_line))
					if (temp_line.size() > 0u && temp_line.at(0) != '#')
						if (first_line_parsed)
							parse_line(temp_line);
						else {
							parse_first_line(temp_line);
							first_line_parsed = true;
						}
			}
}

#include "Settings.hpp"
void ObjectStorage::parse_object_line(std::string const& line) {
	switch (m_current_object) {
	case ObjectType::ClientSettings:
		if (m_objects->isClient()) m_objects->m_settings->parse_line(line);
		break;
	case ObjectType::ServerSettings:
		if (m_objects->isServer()) m_objects->m_settings->parse_line(line);
		break;
	default:
		throw Exceptions::FileParsingException("Unsupported object type was encountered.");
		break;
	}
}