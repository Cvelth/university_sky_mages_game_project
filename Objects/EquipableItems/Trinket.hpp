#pragma once
#include "Objects/Objects/EquipableItem.hpp"

class ObjectStorage;
class Trinket : public EquipableItem {
	friend ObjectStorage;
protected:
	Trinket() : EquipableItem() {}
public:

private:
	bool clear_value(std::string const& name);
	template <typename value_type>
	bool upgrade_value(std::string const& name, value_type const& value);
	template <typename value_type>
	void set_value(std::string const& name, value_type const& value);
};

inline bool Trinket::clear_value(std::string const& name) {
	if (name == "name")
		m_name = "";
	else if (name == "description")
		m_description = "";
	else if (name == "mass")
		mulMass(0.0);
	else if (name == "chance_to_take_damage")
		m_chance_to_take_damage = 0.f;
	else
		return false;
	return true;
}

template<>
inline bool Trinket::upgrade_value<std::string>(std::string const& name, std::string const& value) {
	if (name == "name")
		m_name += value;
	else if (name == "description")
		m_description += value;
	else
		return false;
	return true;
}
template<>
inline bool Trinket::upgrade_value<float>(std::string const& name, float const& value) {
	if (name == "mass")
		addMass(value);
	else if (name == "chance_to_take_damage")
		m_chance_to_take_damage += value;
	else
		return false;
	return true;
}
template<typename value_type>
inline bool Trinket::upgrade_value(std::string const& name, value_type const& value) {
	return false;
}

template<typename value_type>
inline void Trinket::set_value(std::string const& name, value_type const& value) {
	bool clear_flag = clear_value(name);
	bool set_flag = upgrade_value(name, value);
	if (!(clear_flag && set_flag))
		throw Exceptions::UnsupportedValueException("Unsupported value was passed");
}