#pragma once
#include <string>
#include "Objects/ObjectState/DependentObject.hpp"
class EquipableItem : public DependentObjectState {
protected:
	std::string m_name;
	std::string m_description;

	EquipableItem() : DependentObjectState(0.f) {}
public:
	EquipableItem(EquipableItem const& other) : DependentObjectState(other.mass()) {
		m_name = other.m_name;
		m_description = other.m_description;
	}
	~EquipableItem() {}
	inline std::string const& name() const { return m_name; }
	inline std::string const& description() const { return m_description; }
};

#include "Shared/AbstractException.hpp"
DefineNewException(UnsupportedValueException)