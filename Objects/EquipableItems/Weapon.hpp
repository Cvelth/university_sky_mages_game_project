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
	float m_fire_rate;
	float m_ammo_speed;
	float m_ammo_capacity;
	float m_ammo_mass;
	float m_ammo_size_h;
	float m_ammo_size_v;
	float m_reload_time;
	float m_shoot_cost;

	size_t m_current_ammo;
	unsigned long long m_last_shot_time;
	bool is_activated;
public:
	Weapon(AmmoType type, WeaponSize size, float mass)
		: EquipableItem(mass), m_ammo_type(type), m_size(size), is_activated(false), 
		m_last_shot_time(0ull) {}
	~Weapon() {}

	inline AmmoType ammoType() {
		return m_ammo_type;
	}
	inline WeaponSize size() {
		return m_size;
	}
	inline void activate() {
		is_activated = true;
	}
	inline void deactivate() {
		is_activated = false;
	}
	inline bool is_ready() const {
		return is_activated && is_reloaded() && m_current_ammo > 0;
	}
	bool is_reloaded() const;
	ShootableObject* shoot(float current_x, float current_y, float destination_x, float destination_y);
};
