#pragma once
#include "Objects/AbstractObjects/EquipableItem.hpp"

class ObjectStorage;
class EnergyStorage : public EquipableItem {
	friend ObjectStorage;
protected:
	float const m_maximum_capacity;
	float m_current_capacity;
	float m_energy_usage_coefficient;
	float m_energy_percent_loss_per_second;

	mutable unsigned long long m_energy_loss_time;
protected:
	void calculate_energy_loss();
	EnergyStorage(float capacity);
public:
	~EnergyStorage() {}

	[[deprecated]] virtual void make_empty() {
		m_current_capacity = 0.f;
	}
	virtual bool isEmpty() const {
		return m_current_capacity == 0.f;
	}
	virtual float getCapacityPercent() {
		calculate_energy_loss();
		return m_current_capacity / m_maximum_capacity;
	}
	virtual float getCapacityValue() {
		calculate_energy_loss();
		return m_current_capacity;
	}

	virtual bool use(float const amount) {
		calculate_energy_loss();
		m_current_capacity -= amount / m_energy_usage_coefficient;
		if (m_current_capacity < 0.f) {
			m_current_capacity = 0.f;
			return false;
		} else
			return true;
	}
	virtual void update(float const capacity) {
		calculate_energy_loss();
		m_current_capacity = capacity;
	}
private:
	template <typename value_type> 
	void set_value(std::string const& name, value_type const& value);
};

template<>
inline void EnergyStorage::set_value<std::string>(std::string const& name, std::string const& value) {
	if (name == "name")
		m_name = value;
	else if (name == "description")
		m_description = value;
	else
		throw Exceptions::UnsupportedValueException("Unsupported value was passed");
}
template<>
inline void EnergyStorage::set_value<float>(std::string const& name, float const& value) {
	if (name == "mass")
		addMass(value);
	else if (name == "energy_usage_coefficient")
		m_energy_usage_coefficient = value;
	else if (name == "energy_percent_loss_per_second")
		m_energy_percent_loss_per_second = value;
	else
		throw Exceptions::UnsupportedValueException("Unsupported value was passed");
}

template<typename value_type>
inline void EnergyStorage::set_value(std::string const& name, value_type const& value) {
	static_assert("Unsupported value type");
}
