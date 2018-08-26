#pragma once
#include "Objects/AbstractObjects/EquipableItem.hpp"

class ObjectStorage;
class Trinket : public EquipableItem {
	friend ObjectStorage;
protected:
	Trinket() : EquipableItem() {}
public:

private:
	template <typename value_type>
	void set_value(std::string const& name, value_type const& value);
};

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
	if (name == "mass")
		addMass(value);
	else if (name == "chance_to_take_damage")
		m_chance_to_take_damage = value;
	else
		throw Exceptions::UnsupportedValueException("Unsupported value was passed");
}
template<typename value_type>
inline void Trinket::set_value(std::string const& name, value_type const& value) {
	static_assert("Unsupported value type");
}
