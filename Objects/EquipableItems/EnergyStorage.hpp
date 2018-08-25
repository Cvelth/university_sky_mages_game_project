#pragma once
#include "Objects/Objects/EquipableItem.hpp"

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
	EnergyStorage(EnergyStorage const& other);
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
	bool clear_value(std::string const& name);
	template <typename value_type>
	bool upgrade_value(std::string const& name, value_type const& value);
	template <typename value_type> 
	void set_value(std::string const& name, value_type const& value);
};

inline bool EnergyStorage::clear_value(std::string const& name) {
	if (name == "name") {
		m_name = "";
		return true;
	} else if (name == "description") {
		m_description = "";
		return true;
	} else if (name == "mass") {
		mulMass(0.0);
		return true;
	} else if (name == "chance_to_take_damage") {
		m_chance_to_take_damage = 0.f;
		return true;
	} else if (name == "energy_usage_coefficient") {
		m_energy_usage_coefficient = 0.f;
		return true;
	} else if (name == "energy_percent_loss_per_second") {
		m_energy_percent_loss_per_second = 0.f;
		return true;
	}
	return false;
}

template<>
inline bool EnergyStorage::upgrade_value<std::string>(std::string const& name, std::string const& value) {
	if (name == "name") {
		m_name += value;
		return true;
	} else if (name == "description") {
		m_description += value;
		return true;
	}
	return false;
}
template<>
inline bool EnergyStorage::upgrade_value<float>(std::string const& name, float const& value) {
	if (name == "mass") {
		addMass(value);
		return true;
	} else if (name == "chance_to_take_damage") {
		m_chance_to_take_damage += value;
		return true;
	} else if (name == "energy_usage_coefficient") {
		m_energy_usage_coefficient += value;
		return true;
	} else if (name == "energy_percent_loss_per_second") {
		m_energy_percent_loss_per_second += value;
		return true;
	}
	return false;
}
template<typename value_type>
inline bool EnergyStorage::upgrade_value(std::string const& name, value_type const& value) {
	return false;
}

template<typename value_type>
inline void EnergyStorage::set_value(std::string const& name, value_type const& value) {
	bool clear_flag = clear_value(name);
	bool set_flag = upgrade_value(name, value);
	if (!(clear_flag && set_flag))
		throw Exceptions::UnsupportedValueException("Unsupported value was passed");
}