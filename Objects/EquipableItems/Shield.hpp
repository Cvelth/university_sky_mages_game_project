#pragma once
#include "Objects/AbstractObjects/EquipableItem.hpp"
#include "Objects/AbstractObjects/ShootableObject.hpp"

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
	mutable unsigned long long m_energy_usage_time;
protected:
	ShieldGenerator();

	float get_efficience_coefficient(ShootableObjectType type);
public:
	~ShieldGenerator() {}
	void connect_to_energy_source(EnergyStorage *source) { m_energy_source = source; }

	void activate();
	inline void deactivate() const {
		is_activated = false;
	}
	inline bool is_active() const {
		return is_activated;
	}

	bool was_hit(std::shared_ptr<ShootableObject> so);
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
	else if (name == "chance_to_take_damage")
		m_chance_to_take_damage = value;
	else if (name == "anti_energy_efficiency") {
		if (value <= 0.f || value >= 1.f)
			throw Exceptions::UnsupportedValueException(("Bad value of " + name + " was passed.").c_str());
		m_anti_energy_efficiency = value;
	} else if (name == "anti_projectile_efficiency") {
		if (value <= 0.f || value >= 1.f)
			throw Exceptions::UnsupportedValueException(("Bad value of " + name + " was passed.").c_str());
		m_anti_projectile_efficiency = value;
	} else if (name == "anti_bullet_efficiency") {
		if (value <= 0.f || value >= 1.f)
			throw Exceptions::UnsupportedValueException(("Bad value of " + name + " was passed.").c_str());
		m_anti_bullet_efficiency = value;
	} else if (name == "activation_energy")
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
