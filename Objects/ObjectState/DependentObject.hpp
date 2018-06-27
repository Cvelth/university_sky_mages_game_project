#pragma once
#include "shared/vector.hpp"

class DependentObjectState {
protected:
	scalar m_mass;
public:
	DependentObjectState(scalar mass) : m_mass(mass) {}

	virtual scalar mass() const {
		return m_mass;
	}
	virtual void addMass(scalar const& mass) {
		m_mass += mass;
	}
	virtual void mulMass(scalar const& mass) {
		m_mass *= mass;
	}
};