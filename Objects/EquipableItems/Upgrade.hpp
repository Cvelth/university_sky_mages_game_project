#pragma once
#include "Objects/Objects/EquipableItem.hpp"
#include <unordered_map>

class ObjectStorage;
class Upgrade : public EquipableItem {
	friend ObjectStorage;
protected:
	std::unordered_map<std::string, float> m_values;
protected:
	Upgrade() : EquipableItem() {}
public:
	auto const& operator->() const { return m_values; }
	auto const& operator*() const { return m_values; }
	auto const& get() const { return m_values; }
private:
	template <typename value_type>
	void set_value(std::string const& name, value_type const& value);
};

template <>
inline void Upgrade::set_value<float>(std::string const& name, float const& value) {
	m_values.insert(std::make_pair(name, value));
}

template <typename value_type>
inline void Upgrade::set_value(std::string const& name, value_type const& value) {
	static_assert(true, "Only float-value upgrades are now supported.");
}