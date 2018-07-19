#pragma once
#include "Objects/AbstractObjects/EquipableItem.hpp"

enum class AmmoType {
	Energy, Projectile, Bullet
};
enum class WeaponSize {
	Small, One_Arm, One_And_A_Half_Arm, Two_Arm, Big
};
class ShootableObject;
class Weapon : public EquipableItem {
private:
	AmmoType m_ammo_type;
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
	float m_energy_efficency_multiplier;

	mutable size_t m_current_ammo;
	mutable unsigned long long m_last_shot_time;
	mutable bool is_activated;
public:
	Weapon(AmmoType type, WeaponSize size, float mass)
		: EquipableItem(mass), m_ammo_type(type), m_size(size), is_activated(false),
		m_last_shot_time(0ull) {}
	~Weapon() {}

	inline AmmoType ammoType() const {
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
