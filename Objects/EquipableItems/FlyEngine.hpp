#pragma once
#include "Objects\AbstractObjects\EquipableItem.hpp"
#include "Objects\ObjectState\DependedAcceleratableObject.hpp"
#include "Shared\AbstractException.hpp"
DefineNewException(IncorrectInitializationDataWasPassedException);
DefineNewException(YouMustNotUseThisFunctionExceptions)
#define invert_if(inverted, value) (inverted ? +value : -value)

struct FlyEngineControls {
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

class EnergyStorage;
class FlyEngine : public EquipableItem, public DependedAcceleratableObjectState {
private:

protected:
	float m_maximum_acceleration;
	float m_energy_usage_coefficient;

	EnergyStorage* m_energy_source;
	FlyEngineControls m_settings;
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
	FlyEngine(EnergyStorage* energy_source, float energy_usage_coefficient,
			  float max_acceleration, float mass, FlyEngineControls settings)
		: EquipableItem(mass), DependedAcceleratableObjectState(mass), m_energy_source(energy_source), 
		m_energy_usage_coefficient(energy_usage_coefficient), m_settings(settings),
		m_maximum_acceleration(max_acceleration), m_anti_gravity_expected_mass(0.f) {

		if (!m_settings.check_correction())
			throw Exceptions::IncorrectInitializationDataWasPassedException("Data, used to initialize new AbstractFlyEngine, isn't supported.");
	}
	~FlyEngine() {}

	using EquipableItem::mass;
	using EquipableItem::addMass;
	using EquipableItem::mulMass;

	virtual void accelerate_up(bool inverted = false) {
		auto acceleration = calculate_acceleration_up();
		accelerate_v(invert_if(!inverted, acceleration));
	}
	virtual void accelerate_down(bool inverted = false) {
		auto acceleration = calculate_acceleration_down();
		accelerate_v(invert_if(!inverted, acceleration));
	}
	virtual void accelerate_left(bool inverted = false) {
		auto acceleration = calculate_acceleration_left();
		accelerate_h(invert_if(!inverted, acceleration));
	}
	virtual void accelerate_right(bool inverted = false) {
		auto acceleration = calculate_acceleration_right();
		accelerate_h(invert_if(!inverted, acceleration));
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

	virtual vector acceleration(scalar const& time_correct = 1.f) const override;
	
	virtual vector acceleration(scalar const& mass, scalar const& time_correct) const;
	
};