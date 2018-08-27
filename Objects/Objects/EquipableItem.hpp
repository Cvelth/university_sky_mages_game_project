#pragma once
#include <string>
#include "Objects/Objects/DependentObjectState.hpp"
class EquipableItem : public DependentObjectState {
protected:
	std::string m_name;
	std::string m_description;

	float m_chance_to_take_damage;

	EquipableItem() : DependentObjectState(0.f) {}
public:
	EquipableItem(EquipableItem const& other) : DependentObjectState(other.mass()) {
		m_name = other.m_name;
		m_description = other.m_description;
		m_chance_to_take_damage = other.m_chance_to_take_damage;
	}
	~EquipableItem() {}
	inline std::string const& name() const { return m_name; }
	inline std::string const& description() const { return m_description; }
	inline float const chance_to_take_damage() const { return m_chance_to_take_damage; }
};

#include "Shared/AbstractException.hpp"
DefineNewException(UnsupportedValueException)