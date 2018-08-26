#pragma once
#include <string>
class FileLoader {
protected:
	virtual void parse_line(std::string const& line) abstract;
	virtual void parse_first_line(std::string const& line, std::string const& file_extention);
	virtual void parse_file_type_info(std::string const& line) abstract;
	virtual void finalize_parsing() {}

	virtual std::string generate_first_line(std::string const& filename, std::string const& file_extention);
	virtual std::string generate_file_type_info() abstract;
public:
	virtual void load(std::string const& path, std::string const& file_extention);
};

#include "../../Shared/AbstractException.hpp"
DefineNewException(FileCannotBeOpennedException);
DefineNewException(FileParsingException);
DefineNewException(FileVersionException); 
DefineNewException(ProgramVersionException);