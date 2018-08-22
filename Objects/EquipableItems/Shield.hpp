#pragma once
#include "Objects/AbstractObjects/EquipableItem.hpp"

class ShootableObject;
class EnergyStorage;
class ObjectStorage;
class ShieldGenerator : public EquipableItem {
	friend ObjectStorage;
private:
	float m_anti_energy_efficiency;
	float m_anti_projectile_efficiency;
	float m_anti_bullet_efficiency;

	float m_activation_energy;
	float m_energy_per_second;
	EnergyStorage *m_energy_source;

	mutable bool is_activated;
protected:
	ShieldGenerator() : EquipableItem() {}
public:
	~ShieldGenerator() {}
	void connect_to_energy_source(EnergyStorage *source) { m_energy_source = source; }

	inline void activate() const {
		is_activated = true;
	}
	inline void deactivate() const {
		is_activated = false;
	}
	void was_hit(ShootableObject *so);
	void shield() const;
private:
	template <typename value_type>
	void set_value(std::string const& name, value_type const& value);
};

template<>
inline void ShieldGenerator::set_value<std::string>(std::string const& name, std::string const& value) {
	if (name == "name")
		m_name = value;
	else if (name == "description")
		m_description = value;
	else
		throw Exceptions::UnsupportedValueException("Unsupported value was passed");
}
template<>
inline void ShieldGenerator::set_value<float>(std::string const& name, float const& value) {
	if (name == "mass")
		addMass(value);
	else if (name == "anti_energy_efficiency")
		m_anti_energy_efficiency = value;
	else if (name == "anti_projectile_efficiency")
		m_anti_projectile_efficiency = value;
	else if (name == "anti_bullet_efficiency")
		m_anti_bullet_efficiency = value;
	else if (name == "activation_energy")
		m_activation_energy = value;
	else if (name == "energy_per_second")
		m_energy_per_second = value;
	else
		throw Exceptions::UnsupportedValueException("Unsupported value was passed");
}
template<typename value_type>
inline void ShieldGenerator::set_value(std::string const& name, value_type const& value) {
	static_assert("Unsupported value type");
}
