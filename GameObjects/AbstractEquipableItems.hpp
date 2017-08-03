#pragma once
enum class EquipmentType {
	Weapon, Shield_generator, Energy_storage, 
	Fly_engine, Trinket
};
class AbstractEquipableItem {
private:
	EquipmentType m_type;
protected:
	float m_mass;
public:
	AbstractEquipableItem(EquipmentType type, float mass)
		: m_type(type), m_mass(mass) {}
	~AbstractEquipableItem() {}

	inline EquipmentType type() {
		return m_type;
	}
	inline float mass() {
		return m_mass;
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

class AbstractFlyEngine : public AbstractEquipableItem {
private:

protected:
	float m_maximum_acceleration;
	float m_energy_usage;
public:
	AbstractFlyEngine(float energy_usage, float max_acceleration, float mass)
		: AbstractEquipableItem(EquipmentType::Fly_engine, mass), 
		m_maximum_acceleration(max_acceleration), m_energy_usage(energy_usage) {}
	~AbstractFlyEngine() {}
};

class AbstractTrinket : public AbstractEquipableItem {
private:

protected:

public:
	AbstractTrinket(float mass) 
		: AbstractEquipableItem(EquipmentType::Trinket, mass) {}
	~AbstractTrinket() {}
};