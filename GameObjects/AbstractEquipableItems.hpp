#pragma once
#include "ObjectState.hpp"
enum class EquipmentType {
	Weapon, Shield_generator, Energy_storage, 
	Fly_engine, Trinket
};
class AbstractEquipableItem : public DependentObjectState {
private:
	EquipmentType m_type;
public:
	AbstractEquipableItem(EquipmentType type, float mass)
		: DependentObjectState(mass), m_type(type) {}
	~AbstractEquipableItem() {}

	inline EquipmentType type() {
		return m_type;
	}
};

enum class WeaponType {
	Energy, Projectile, Bullet
};
enum class WeaponSize {
	Small, One_Arm, One_And_A_Half_Arm, Two_Arm, Big
};
class AbstractWeapon : public AbstractEquipableItem {
private:
	WeaponType m_type;
	WeaponSize m_size;
protected:
	float m_damage;
	float m_fire_rate;
	float m_ammo_speed;
	float m_ammo_capacity;
	float m_reload_time;
	float m_shoot_cost;
public:
	AbstractWeapon(WeaponType type, WeaponSize size, float mass) 
		: AbstractEquipableItem(EquipmentType::Weapon, mass), m_type(type), m_size(size) {}
	~AbstractWeapon() {}
};

enum class ShieldType {
	Anti_Energy, Anti_Projectile, Anti_Bullet
};
class AbstractShieldGenerator : public AbstractEquipableItem {
private:
	ShieldType m_type;
protected:
	
public:
	AbstractShieldGenerator(ShieldType type, float mass) 
		: AbstractEquipableItem(EquipmentType::Shield_generator, mass) {}
	~AbstractShieldGenerator() {}
};

class AbstractEnergyStorage : public AbstractEquipableItem {
private:

protected:
	float m_capacity;
public:
	AbstractEnergyStorage(float capacity, float mass) 
		: AbstractEquipableItem(EquipmentType::Energy_storage, mass), m_capacity(capacity) {}
	~AbstractEnergyStorage() {}
};

class AbstractFlyEngine : public AbstractEquipableItem, public DependedAcceleratableObjectState {
private:

protected:
	float m_maximum_acceleration;
	float m_energy_usage;
public:
	AbstractFlyEngine(float energy_usage, float max_acceleration, float mass)
		: AbstractEquipableItem(EquipmentType::Fly_engine, mass), DependedAcceleratableObjectState(mass),
		m_maximum_acceleration(max_acceleration), m_energy_usage(energy_usage) {}
	~AbstractFlyEngine() {}

	using AbstractEquipableItem::mass;
	using AbstractEquipableItem::addMass;
	using AbstractEquipableItem::mulMass;

	virtual void accelerate_max(vector const& direction) {
		vector temp(0.f, 0.f);
		if (direction.h != 0.f)
			temp.h = direction.h > 0.f ? 1.f : -1.f;
		if (direction.v != 0.f)
			temp.v = direction.v > 0.f ? 1.f : -1.f;
		m_acceleration = direction * m_maximum_acceleration;
	}
	virtual void accelerate_h_max(bool sign = true) {
		if (sign)
			accelerate_h(+m_maximum_acceleration);
		else
			accelerate_h(-m_maximum_acceleration);
	}
	virtual void accelerate_v_max(bool sign = true) {
		if (sign)
			accelerate_v(+m_maximum_acceleration);
		else
			accelerate_v(-m_maximum_acceleration);
	}
};

class AbstractTrinket : public AbstractEquipableItem {
private:

protected:

public:
	AbstractTrinket(float mass) 
		: AbstractEquipableItem(EquipmentType::Trinket, mass) {}
	~AbstractTrinket() {}
};