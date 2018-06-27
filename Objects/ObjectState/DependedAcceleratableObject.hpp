#pragma once
#include "Objects/ObjectState/DependentObject.hpp"

class DependedAcceleratableObjectState : public DependentObjectState {
protected:
	vector m_acceleration;
public:
	DependedAcceleratableObjectState(scalar mass, scalar m_acceleration_h = 0.f, scalar m_acceleration_v = 0.f)
		: DependentObjectState(mass), m_acceleration(m_acceleration_h, m_acceleration_v) {}

	virtual vector acceleration(scalar const& time_correct = 1.f) const {
		return m_acceleration;
	}
	virtual void accelerate(vector const& difference) {
		m_acceleration += difference;
	}
	virtual void accelerate_h(scalar const& acceleration) {
		m_acceleration[0] += acceleration;
	}
	virtual void accelerate_v(scalar const& acceleration) {
		m_acceleration[1] += acceleration;
	}
	virtual void stopAcceleration_h() {
		m_acceleration[0] = 0.f;
	}
	virtual void stopAcceleration_v() {
		m_acceleration[1] = 0.f;
	}
};