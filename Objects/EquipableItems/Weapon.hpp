#pragma once
#include "Shared/AbstractException.hpp"
#include "Objects/AbstractObjects/EquipableItem.hpp"

enum class AmmoProjectileType {
	Energy, Physical, Bullet
};
DefineNewException(UnsupportedProjectileType);
enum class WeaponSize {
	Small, One_Arm, One_And_A_Half_Arm, Two_Arm, Big
};
DefineNewException(UnsupportedWeaponSize);

class ShootableObject;
class EnergyStorage;
class Weapon : public EquipableItem {
private:
	AmmoProjectileType m_ammo_type;
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
public:
	Weapon(AmmoProjectileType type, WeaponSize size, float mass, EnergyStorage* energy_source, float energy_usage_coefficient)
		: EquipableItem(mass), m_ammo_type(type), m_size(size), is_activated(false), m_last_shot_time(0ull), 
		m_energy_source(energy_source), m_energy_usage_coefficient(energy_usage_coefficient) {}
	~Weapon() {}

	inline AmmoProjectileType ammoType() const {
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
	ShootableObject* shoot(float current_x, float current_y, float destination_x, float destination_y) const;
};
