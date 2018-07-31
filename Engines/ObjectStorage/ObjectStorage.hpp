#pragma once
#include <string>
/*
Supported object types:
- Client Settings
- Server Settings
*/
enum class ObjectType {
	Empty = 0, ClientSettings, ServerSettings
};

class ObjectStorage {
private:
	ObjectType m_current_object;
	size_t m_current_object_counter, m_current_object_number;
protected:
	void parse_line(std::string const& line);
	void parse_object_line(std::string const& line);
	void parse_first_line(std::string const& line);
public:
	void load(std::string const& path = "/");
	ObjectStorage(std::string const& path = "/") 
		: m_current_object(ObjectType::Empty), m_current_object_counter(0), 
		m_current_object_number(0) { load(path); }
};

#include "../../Shared/AbstractException.hpp"
DefineNewException(FileCannotBeOpennedException);
DefineNewException(FileParsingException);
DefineNewException(FileVersionException);