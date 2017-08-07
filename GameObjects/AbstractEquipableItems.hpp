#pragma once
#include "ObjectState.hpp"
#include "PhysicsEngine\PhysicalConstants.hpp"
#include "Shared\AbstractException.hpp"
DefineNewException(IncorrectInitializationDataWasPassedException);

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

	virtual void make_empty() {
		m_capacity = 0.f;
	}
	virtual bool isEmpty() const {
		return m_capacity == 0.f;
	}
};

struct AbstractFlyEngineSettings {
	float anti_gravity_mode_off_up_acceleration_percent;
	float anti_gravity_mode_off_down_acceleration_percent;
	float anti_gravity_mode_off_left_acceleration_percent;
	float anti_gravity_mode_off_right_acceleration_percent;

	float anti_gravity_mode_on_up_acceleration_percent;
	float anti_gravity_mode_on_down_acceleration_percent;
	float anti_gravity_mode_on_left_acceleration_percent;
	float anti_gravity_mode_on_right_acceleration_percent;

	bool check_correction() {
		return (anti_gravity_mode_off_up_acceleration_percent < 1.f || anti_gravity_mode_off_up_acceleration_percent > 0.f) &&
			(anti_gravity_mode_off_down_acceleration_percent < 1.f || anti_gravity_mode_off_down_acceleration_percent > 0.f) &&
			(anti_gravity_mode_off_left_acceleration_percent < 1.f || anti_gravity_mode_off_left_acceleration_percent > 0.f) &&
			(anti_gravity_mode_off_right_acceleration_percent < 1.f || anti_gravity_mode_off_right_acceleration_percent > 0.f) &&

			(anti_gravity_mode_on_up_acceleration_percent < 1.f || anti_gravity_mode_on_up_acceleration_percent > 0.f) &&
			(anti_gravity_mode_on_down_acceleration_percent < 1.f || anti_gravity_mode_on_down_acceleration_percent > 0.f) &&
			(anti_gravity_mode_on_left_acceleration_percent < 1.f || anti_gravity_mode_on_left_acceleration_percent > 0.f) &&
			(anti_gravity_mode_on_right_acceleration_percent < 1.f || anti_gravity_mode_on_right_acceleration_percent > 0.f);
	}
};

class AbstractFlyEngine : public AbstractEquipableItem, public DependedAcceleratableObjectState {
private:

protected:
	float m_maximum_acceleration;
	float m_energy_usage;

	AbstractEnergyStorage* m_energy_source;
	AbstractFlyEngineSettings m_settings;
	float m_anti_gravity_expected_mass; //0.f for turned off.
protected:
	float const calculate_acceleration_up() const {
		if (m_anti_gravity_expected_mass)
			return -m_maximum_acceleration * m_settings.anti_gravity_mode_on_up_acceleration_percent;
		else
			return -m_maximum_acceleration * m_settings.anti_gravity_mode_off_up_acceleration_percent;
	}
	float const calculate_acceleration_down() const {
		if (m_anti_gravity_expected_mass)
			return +m_maximum_acceleration * m_settings.anti_gravity_mode_on_down_acceleration_percent;
		else
			return +m_maximum_acceleration * m_settings.anti_gravity_mode_off_down_acceleration_percent;
	}
	float const calculate_acceleration_left() const {
		if (m_anti_gravity_expected_mass)
			return -m_maximum_acceleration * m_settings.anti_gravity_mode_on_left_acceleration_percent;
		else
			return -m_maximum_acceleration * m_settings.anti_gravity_mode_off_left_acceleration_percent;
	}
	float const calculate_acceleration_right() const {
		if (m_anti_gravity_expected_mass)
			return +m_maximum_acceleration * m_settings.anti_gravity_mode_on_right_acceleration_percent;
		else
			return +m_maximum_acceleration * m_settings.anti_gravity_mode_off_right_acceleration_percent;
	}
public:
	AbstractFlyEngine(AbstractEnergyStorage* energy_source, 
					  float energy_usage, float max_acceleration, float mass,
					  AbstractFlyEngineSettings settings)
		: AbstractEquipableItem(EquipmentType::Fly_engine, mass), DependedAcceleratableObjectState(mass),
		m_energy_source(energy_source), m_energy_usage(energy_usage), m_settings(settings),
		m_maximum_acceleration(max_acceleration), m_anti_gravity_expected_mass(0.f) {
	
		if (!m_settings.check_correction())
			throw Exceptions::IncorrectInitializationDataWasPassedException("Data, used to initialize new AbstractFlyEngine, isn't supported.");
	}
	~AbstractFlyEngine() {}

	using AbstractEquipableItem::mass;
	using AbstractEquipableItem::addMass;
	using AbstractEquipableItem::mulMass; 

	virtual void accelerate_up(bool inverted = false) {
		if (!inverted)
			accelerate_v(calculate_acceleration_up());
		else
			accelerate_v(-calculate_acceleration_up());
	}
	virtual void accelerate_down(bool inverted = false) {
		if (!inverted)
			accelerate_v(calculate_acceleration_down());
		else
			accelerate_v(-calculate_acceleration_down());
	}
	virtual void accelerate_left(bool inverted = false) {
		if (!inverted)
			accelerate_h(calculate_acceleration_left());
		else
			accelerate_h(-calculate_acceleration_left());
	}
	virtual void accelerate_right(bool inverted = false) {
		if (!inverted)
			accelerate_h(calculate_acceleration_right());
		else
			accelerate_h(-calculate_acceleration_right());
	}
	void turn_on_anti_gravity(float expected_mass) {
		m_anti_gravity_expected_mass = expected_mass;
	}
	void turn_off_anti_gravity() {
		m_anti_gravity_expected_mass = 0.f;
	}
	bool is_auti_gravity_turned_on() {
		return m_anti_gravity_expected_mass != 0.f;
	}

	virtual vector acceleration() const {
		auto ret = DependedAcceleratableObjectState::acceleration();
		ret.v -= m_anti_gravity_expected_mass * Constants::g;
		return ret;
	}
	virtual vector acceleration(scalar const& mass) const { 
		//TO_REWRITE
		auto ret = DependedAcceleratableObjectState::acceleration();
		ret.v -= m_anti_gravity_expected_mass * Constants::g;
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