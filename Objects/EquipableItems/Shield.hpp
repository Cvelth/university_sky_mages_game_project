#pragma once
#include "Objects/Objects/EquipableItem.hpp"
#include "Objects/Objects/ShootableObject.hpp"

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
	std::shared_ptr<EnergyStorage> m_energy_source;

	mutable bool is_activated;
	mutable unsigned long long m_energy_usage_time;
protected:
	ShieldGenerator();

	float get_efficience_coefficient(ShootableObjectType type);
public:
	~ShieldGenerator() {}
	void connect_to_energy_source(std::shared_ptr<EnergyStorage> source) { m_energy_source = source; }

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
	bool clear_value(std::string const& name);
	template <typename value_type>
	bool upgrade_value(std::string const& name, value_type const& value);
	template <typename value_type>
	void set_value(std::string const& name, value_type const& value);
};

inline bool ShieldGenerator::clear_value(std::string const& name) {
	if (name == "name")
		m_name = "";
	else if (name == "description")
		m_description = "";
	else if (name == "mass")
		mulMass(0.0);
	else if (name == "chance_to_take_damage")
		m_chance_to_take_damage = 0.f;
	else if (name == "anti_energy_efficiency")
		m_anti_energy_efficiency = 0.f;
	else if (name == "anti_projectile_efficiency")
		m_anti_projectile_efficiency = 0.f;
	else if (name == "anti_bullet_efficiency")
		m_anti_bullet_efficiency = 0.f;
	else if (name == "activation_energy")
		m_activation_energy = 0.f;
	else if (name == "energy_per_second")
		m_energy_per_second = 0.f;
	else
		return false;
	return true;
}

template<>
inline bool ShieldGenerator::upgrade_value<std::string>(std::string const& name, std::string const& value) {
	if (name == "name")
		m_name += value;
	else if (name == "description")
		m_description += value;
	else
		return false;
	return true;
}
template<>
inline bool ShieldGenerator::upgrade_value<float>(std::string const& name, float const& value) {
	if (name == "mass")
		addMass(value);
	else if (name == "chance_to_take_damage")
		m_chance_to_take_damage += value;
	else if (name == "anti_energy_efficiency")
		m_anti_energy_efficiency += value;
	else if (name == "anti_projectile_efficiency")
		m_anti_projectile_efficiency += value;
	else if (name == "anti_bullet_efficiency")
		m_anti_bullet_efficiency += value;
	else if (name == "activation_energy")
		m_activation_energy += value;
	else if (name == "energy_per_second")
		m_energy_per_second += value;
	else
		return false;
	return true;
}
template<typename value_type>
inline bool ShieldGenerator::upgrade_value(std::string const& name, value_type const& value) {
	return false;
}

template<typename value_type>
inline void ShieldGenerator::set_value(std::string const& name, value_type const& value) {
	bool clear_flag = clear_value(name);
	bool set_flag = upgrade_value(name, value);
	if (!(clear_flag && set_flag))
		throw Exceptions::UnsupportedValueException("Unsupported value was passed");
}