#pragma once
#include "Objects/Objects/EquipableItem.hpp"
#include "Objects/Objects/ShootableObject.hpp"
class EnergyStorage;
class ObjectStorage;
class MainActor;

enum class WeaponSize {
	None = 0, Small = 3, One_Arm = 1, One_And_A_Half_Arm = 4, Two_Arm = 2, Big = 5
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
	std::string m_ammo_render_info;

	size_t m_ammo_capacity;
	float m_reload_cooldown;

	float m_energy_usage_coefficient;
	std::shared_ptr<EnergyStorage> m_energy_source;

	mutable size_t m_current_ammo;
	mutable unsigned long long m_last_shot_time;
	mutable bool is_activated;

	Weapon() : EquipableItem() {}
public:
	~Weapon() {}
	void connect_to_energy_source(std::shared_ptr<EnergyStorage> source) { m_energy_source = source; }

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
	bool clear_value(std::string const& name);
	template <typename value_type>
	bool upgrade_value(std::string const& name, value_type const& value);
	template <typename value_type>
	void set_value(std::string const& name, value_type const& value);
};

inline bool Weapon::clear_value(std::string const& name) {
	if (name == "name")
		m_name = "";
	else if (name == "description")
		m_description = "";
	else if (name == "ammo_render_info")
		m_ammo_render_info = "";
	else if (name == "ammo_type")
		m_ammo_type = ShootableObjectType::None;
	else if (name == "size")
		m_size = WeaponSize::None;
	else if (name == "ammo_capacity") {
		m_ammo_capacity = 0u;
		m_current_ammo = 0u;
	} else if (name == "autofire_support")
		m_autofire_supported = false; 
	else if (name == "mass")
		mulMass(0.0);
	else if (name == "chance_to_take_damage")
		m_chance_to_take_damage = 0.f;
	else if (name == "energy_usage_coefficient")
		m_energy_usage_coefficient = 0.f;
	else if (name == "damage")
		m_damage = 0.f;
	else if (name == "firerate")
		m_firerate = 0.f;
	else if (name == "initial_ammo_speed")
		m_initial_ammo_speed = 0.f;
	else if (name == "initial_ammo_mass")
		m_initial_ammo_mass = 0.f;
	else if (name == "initial_ammo_size_h")
		m_initial_ammo_size_h = 0.f;
	else if (name == "initial_ammo_size_v")
		m_initial_ammo_size_v = 0.f;
	else if (name == "reload_cooldown")
		m_reload_cooldown = 0.f;
	else
		return false;
	return true;
}

template<>
inline bool Weapon::upgrade_value<std::string>(std::string const& name, std::string const& value) {
	if (name == "name")
		m_name += value;
	else if (name == "description")
		m_description += value;
	else if (name == "ammo_render_info")
		m_ammo_render_info += value;
	else
		return false;
	return true;
}
template<>
inline bool Weapon::upgrade_value<unsigned int>(std::string const& name, unsigned int const& value) {
	if (name == "ammo_type")
		m_ammo_type = static_cast<ShootableObjectType>(value);
	else if (name == "size")
		m_size = static_cast<WeaponSize>(value);
	else if (name == "ammo_capacity") {
		m_ammo_capacity += value;
		m_current_ammo += value;
	} else
		return false;
	return true;
}
template<>
inline bool Weapon::upgrade_value<bool>(std::string const& name, bool const& value) {
	if (name == "autofire_support")
		m_autofire_supported = value;
	else
		return false;
	return true;
}
template<>
inline bool Weapon::upgrade_value<float>(std::string const& name, float const& value) {
	if (name == "mass")
		addMass(value);
	else if (name == "chance_to_take_damage")
		m_chance_to_take_damage += value;
	else if (name == "energy_usage_coefficient")
		m_energy_usage_coefficient += value;
	else if (name == "damage")
		m_damage += value;
	else if (name == "firerate")
		m_firerate += value;
	else if (name == "initial_ammo_speed")
		m_initial_ammo_speed += value;
	else if (name == "initial_ammo_mass")
		m_initial_ammo_mass += value;
	else if (name == "initial_ammo_size_h")
		m_initial_ammo_size_h += value;
	else if (name == "initial_ammo_size_v")
		m_initial_ammo_size_v += value;
	else if (name == "reload_cooldown")
		m_reload_cooldown += value;
	else
		return false;
	return true;
}
template<typename value_type>
inline bool Weapon::upgrade_value(std::string const& name, value_type const& value) {
	return false;
}

template<typename value_type>
inline void Weapon::set_value(std::string const& name, value_type const& value) {
	bool clear_flag = clear_value(name);
	bool set_flag = upgrade_value(name, value);
	if (!(clear_flag && set_flag))
		throw Exceptions::UnsupportedValueException("Unsupported value was passed");
}
#include "Shared/AbstractException.hpp"
DefineNewException(UnsupportedWeaponSize);