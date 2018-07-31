#include "ObjectStorage.hpp"
#include <list>
#include <string>

std::list<std::string> split_path(std::string const& path, std::string const& separator = ";") {
	std::list<std::string> ret;

	size_t position, last_position = 0u, length = path.length();

	while (last_position < length + 1) {
		position = path.find_first_of(separator, last_position);
		if (position == std::string::npos)
			position = length;

		if (position != last_position)
			ret.push_back(std::string{path.data() + last_position, position - last_position});
		last_position = position + 1;
	}

	return ret;
}
void ObjectStorage::load(std::string const& path_string) {
	for (auto path : split_path(path_string)) {
	
	}
}
