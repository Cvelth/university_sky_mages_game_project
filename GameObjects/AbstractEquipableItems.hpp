#pragma once
#include "ObjectState.hpp"
#include "PhysicsEngine\PhysicalConstants.hpp"
#include "Shared\AbstractException.hpp"
DefineNewException(IncorrectInitializationVariableWasPassedException);

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

	float m_up_acceleration_percent;
	float m_down_acceleration_percent;
	float m_left_acceleration_percent;
	float m_right_acceleration_percent;

	float anti_gravity_expected_mass; //0.f for turned off.
public:
	AbstractFlyEngine(float energy_usage, float max_acceleration, float mass,
					  float up_percent, float down_percent, float left_percent, float right_percent)
		: AbstractEquipableItem(EquipmentType::Fly_engine, mass), DependedAcceleratableObjectState(mass),
		m_maximum_acceleration(max_acceleration), m_energy_usage(energy_usage),
		m_up_acceleration_percent(up_percent), m_down_acceleration_percent(down_percent),
		m_left_acceleration_percent(left_percent), m_right_acceleration_percent(right_percent), 
		anti_gravity_expected_mass(0.f) {
	
		if ((m_up_acceleration_percent > 1.f || m_up_acceleration_percent < 0.f) &&
			(m_down_acceleration_percent > 1.f || m_down_acceleration_percent < 0.f) &&
			(m_left_acceleration_percent > 1.f || m_left_acceleration_percent < 0.f) &&
			(m_right_acceleration_percent > 1.f || m_right_acceleration_percent < 0.f)) {

			throw Exceptions::IncorrectInitializationVariableWasPassedException();
		}
	}
	~AbstractFlyEngine() {}

	using AbstractEquipableItem::mass;
	using AbstractEquipableItem::addMass;
	using AbstractEquipableItem::mulMass;

	virtual void accelerate_up() {
		accelerate_v(-m_maximum_acceleration * m_up_acceleration_percent);
	}
	virtual void accelerate_down() {
		accelerate_v(m_maximum_acceleration * m_down_acceleration_percent);
	}
	virtual void accelerate_left() {
		accelerate_h(-m_maximum_acceleration * m_left_acceleration_percent);
	}
	virtual void accelerate_right() {
		accelerate_h(m_maximum_acceleration * m_right_acceleration_percent);
	}
	void turn_on_anti_gravity(float expected_mass) {
		anti_gravity_expected_mass = expected_mass;
	}
	void turn_off_anti_gravity() {
		anti_gravity_expected_mass = 0.f;
	}
	bool is_auti_gravity_turned_on() {
		return anti_gravity_expected_mass != 0.f;
	}

	virtual vector acceleration() const {
		auto ret = DependedAcceleratableObjectState::acceleration();
		ret.v -= anti_gravity_expected_mass * Constants::g;
		return ret;
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