#pragma once
#include <string>

class ObjectStorage {
public:
	ObjectStorage(std::string const& path = "/") { load(path); }
	void load(std::string const& path = "/");
};

#include "../../Shared/AbstractException.hpp"
DefineNewException(FileCannotBeOpennedException);