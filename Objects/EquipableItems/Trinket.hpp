#pragma once
#include "Objects/Objects/EquipableItem.hpp"

class ObjectStorage;
class MainActor;
class Trinket : public EquipableItem {
	friend ObjectStorage;
	friend MainActor;
protected:
	Trinket() : EquipableItem() {}
public:

private:
	template <typename value_type>
	bool upgrade_value(std::string const& name, value_type const& value);
	template <typename value_type>
	void set_value(std::string const& name, value_type const& value);
};

template<>
inline bool Trinket::upgrade_value<float>(std::string const& name, float const& value) {
	if (name == "mass")
		mulMass(value);
	else if (name == "chance_to_take_damage")
		m_chance_to_take_damage *= value;
	else
		return false;
	return true;
}
template<typename value_type>
inline bool Trinket::upgrade_value(std::string const& name, value_type const& value) {
	return false;
}

template<>
inline void Trinket::set_value<std::string>(std::string const& name, std::string const& value) {
	if (name == "name")
		m_name = value;
	else if (name == "description")
		m_description = value;
	else
		throw Exceptions::UnsupportedValueException("Unsupported value was passed");
}
template<>
inline void Trinket::set_value<float>(std::string const& name, float const& value) {
	if (name == "mass") {
		mulMass(0.f);
		addMass(value);
	} else if (name == "chance_to_take_damage")
		m_chance_to_take_damage = value;
	else
		throw Exceptions::UnsupportedValueException("Unsupported value was passed");
}
template<typename value_type>
inline void Trinket::set_value(std::string const& name, value_type const& value) {
	throw Exceptions::UnsupportedValueException("Unsupported value was passed");
}