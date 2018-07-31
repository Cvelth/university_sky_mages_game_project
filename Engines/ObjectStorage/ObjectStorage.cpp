#include "ObjectStorage.hpp"
#include "DefaultObjectStorageData.hpp"

#include <list>
#include <string>
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
				size_t number_of_objects = 0u;
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