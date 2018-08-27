#pragma once
#include <list>
#include <sstream>
namespace Functions {
	inline std::list<std::string> split_string(std::string const& string, std::string const& separator = ";") {
		std::list<std::string> ret;
		size_t position, last_position = 0u, length = string.length();
		while (last_position < length + 1) {
			position = string.find_first_of(separator, last_position);
			if (position == std::string::npos) position = length;
			ret.push_back(std::string{string.data() + last_position, position - last_position});
			last_position = position + 1;
		}
		return ret;
	}

	inline std::string string_indexation(std::string const& string, size_t index) {
		std::ostringstream s;
		s << string << '[' << index << ']';
		return s.str();
	}
}