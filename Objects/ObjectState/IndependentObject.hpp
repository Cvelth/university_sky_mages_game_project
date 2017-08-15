#pragma once
#include "Objects\ObjectState\DependedAcceleratableObject.hpp"

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
		m_position[0] = h;
	}
	virtual void move_to_v(scalar const& v) {
		m_position[1] = v;
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