#pragma once
#include "AbstractExceptions.hpp"
namespace Exceptions {
	class SettingsException : public AbstractStringException {
	public: SettingsException(const char* name, const char* desc)
		: AbstractStringException(name, desc) {}
	};
	class FileAccessException : public SettingsException {
	public: FileAccessException()
		: SettingsException("FileAccessException", "File cannot be open.") {}
	};
	class UnsupportedSettingFileException : public SettingsException {
	public: UnsupportedSettingFileException()
		: SettingsException("UnsupportedSettingFileException", "Settings file is not supported.") {}
	};
	class DifferentSettingFileVersionException : public SettingsException {
	public: DifferentSettingFileVersionException()
		: SettingsException("DifferentSettingFileVersionException", "Settings file version isn't equal to supported one.") {}
	};
	class DifferentProgramVersionException : public SettingsException {
	public: DifferentProgramVersionException()
		: SettingsException("DifferentProgramVersionException", "Program version in Settings file isn't equal to current one.") {}
	};
	class UnsupportedSettingTypeException : public SettingsException {
	public: UnsupportedSettingTypeException()
		: SettingsException("UnsupportedSettingTypeException", "Type is not supported.") {}
	};
	class NonExistingSettingException : public SettingsException {
	public: NonExistingSettingException()
		: SettingsException("NonExistingSettingException", "Setting wasn't created/loaded.") {}
	};
}