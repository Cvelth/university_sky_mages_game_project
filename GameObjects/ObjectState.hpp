#pragma once
#include "PhysicsEngine\vector.hpp"

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
		m_acceleration.h += acceleration;
	}
	virtual void accelerate_v(scalar const& acceleration) {
		m_acceleration.v += acceleration;
	}
	virtual void stopAcceleration_h() {
		m_acceleration.h = 0.f;
	}
	virtual void stopAcceleration_v() {
		m_acceleration.v = 0.f;
	}
};

class IndependentObjectState : public DependedAcceleratableObjectState {
protected:
	vector m_size;
	vector m_position;
	vector m_speed;
public:
	IndependentObjectState(scalar mass, scalar size_h, scalar size_v,
						   scalar position_h = 0.f, scalar position_v = 0.f,
						   scalar speed_h = 0.f, scalar speed_v = 0.f,
						   scalar m_acceleration_h = 0.f, scalar m_acceleration_v = 0.f)
		: DependedAcceleratableObjectState(mass, m_acceleration_h, m_acceleration_v),
		m_size(size_h, size_v), m_position(position_h, position_v), m_speed(speed_h, speed_v) {}

	virtual vector size() const {
		return m_size;
	}
	virtual void resize(vector const& difference) {
		m_size += difference;
	}

	virtual vector position() const {
		return m_position;
	}
	virtual void move(vector const& movement) {
		m_position += movement;
	}
	virtual void move_to(vector const& position) {
		m_position = position;
	}
	virtual void move_to_h(scalar const& h) {
		m_position.h = h;
	}
	virtual void move_to_v(scalar const& v) {
		m_position.v = v;
	}

	virtual vector speed() const {
		return m_speed;
	}
	virtual void speed_up(vector const& difference) {
		m_speed += difference;
	}
	virtual void speed_mul(vector const& multiplier) {
		m_speed *= multiplier;
	}

};