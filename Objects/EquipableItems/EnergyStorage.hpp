#pragma once
#include "Objects/AbstractObjects/EquipableItem.hpp"

class EnergyStorage : public EquipableItem {
private:

protected:
	float const m_maximum_capacity;
	float m_current_capacity;
public:
	EnergyStorage(float capacity, float mass) : EquipableItem(mass),
		m_current_capacity(capacity), m_maximum_capacity(capacity) {}
	~EnergyStorage() {}

	virtual void make_empty() {
		m_current_capacity = 0.f;
	}
	virtual bool isEmpty() const {
		return m_current_capacity == 0.f;
	}
	virtual float getCapacityPercent() {
		return float(m_current_capacity) / m_maximum_capacity;
	}

	virtual bool use(float const amount) {
		m_current_capacity -= amount;
		if (m_current_capacity < 0.f) {
			m_current_capacity = 0.f;
			return false;
		} else
			return true;
	}
};