#pragma once
#include <string>
#include "Objects/ObjectState/DependentObject.hpp"
class EquipableItem : public DependentObjectState {
	std::string m_name;
	std::string m_description;
public:
	EquipableItem(float mass, std::string &name, std::string &description)
		: DependentObjectState(mass), m_name(name), m_description(description) {}
	~EquipableItem() {}
	inline std::string const& name() const { return m_name; }
	inline std::string const& description() const { return m_description; }
};