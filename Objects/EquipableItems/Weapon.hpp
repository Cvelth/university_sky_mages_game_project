#pragma once
#include "Objects/AbstractObjects/EquipableItem.hpp"
#include "Objects/AbstractObjects/ShootableObject.hpp"
class EnergyStorage;
class ObjectStorage;
class MainActor;

enum class WeaponSize {
	Small = 3, One_Arm = 1, One_And_A_Half_Arm = 4, Two_Arm = 2, Big = 5
};

class Weapon : public EquipableItem {
	friend ObjectStorage;
private:
	ShootableObjectType m_ammo_type;
	WeaponSize m_size;
protected:
	float m_damage;
	float m_firerate;
	bool m_autofire_supported;

	float m_initial_ammo_speed;
	float m_initial_ammo_mass;
	float m_initial_ammo_size_h;
	float m_initial_ammo_size_v;

	size_t m_ammo_capacity;
	float m_reload_cooldown;

	float m_energy_usage_coefficient;
	EnergyStorage* m_energy_source;

	mutable size_t m_current_ammo;
	mutable unsigned long long m_last_shot_time;
	mutable bool is_activated;

	Weapon() : EquipableItem() {}
public:
	~Weapon() {}
	void connect_to_energy_source(EnergyStorage *source) { m_energy_source = source; }

	inline ShootableObjectType ammoType() const {
		return m_ammo_type;
	}
	inline WeaponSize size() const {
		return m_size;
	}
	inline void activate() const {
		is_activated = true;
	}
	inline void deactivate() const {
		is_activated = false;
	}
	inline bool is_ready() const {
		return is_activated && is_reloaded();
	}
	bool is_reloaded() const;
	std::shared_ptr<ShootableObject> shoot(size_t shooter_id, float current_x, float current_y, float destination_x, float destination_y) const;
private:
	template <typename value_type>
	void set_value(std::string const& name, value_type const& value);
};

#include "Shared/AbstractException.hpp"
DefineNewException(UnsupportedWeaponSize);

template<>
inline void Weapon::set_value<std::string>(std::string const& name, std::string const& value) {
	if (name == "name")
		m_name = value;
	else if (name == "description")
		m_description = value;
	else
		throw Exceptions::UnsupportedValueException("Unsupported value was passed");
}
template<>
inline void Weapon::set_value<unsigned int>(std::string const& name, unsigned int const& value) {
	if (name == "ammo_type")
		m_ammo_type = static_cast<ShootableObjectType>(value);
	else if (name == "size")
		m_size = static_cast<WeaponSize>(value);
	else if (name == "ammo_capacity") {
		m_ammo_capacity = value;
		m_current_ammo = value;
	} else
		throw Exceptions::UnsupportedValueException("Unsupported value was passed");
}
template<>
inline void Weapon::set_value<bool>(std::string const& name, bool const& value) {
	if (name == "autofire_support")
		m_autofire_supported = value;
	else
		throw Exceptions::UnsupportedValueException("Unsupported value was passed");
}
template<>
inline void Weapon::set_value<float>(std::string const& name, float const& value) {
	if (name == "mass")
		addMass(value);
	else if (name == "energy_usage_coefficient")
		m_energy_usage_coefficient = value;
	else if (name == "damage")
		m_damage = value;
	else if (name == "firerate")
		m_firerate = value;
	else if (name == "initial_ammo_speed")
		m_initial_ammo_speed = value;
	else if (name == "initial_ammo_mass")
		m_initial_ammo_mass = value;
	else if (name == "initial_ammo_size_h")
		m_initial_ammo_size_h = value;
	else if (name == "initial_ammo_size_v")
		m_initial_ammo_size_v = value;
	else if (name == "reload_cooldown")
		m_reload_cooldown = value;
	else
		throw Exceptions::UnsupportedValueException("Unsupported value was passed");
}
template<typename value_type>
inline void Weapon::set_value(std::string const& name, value_type const& value) {
	static_assert("Unsupported value type");
}
