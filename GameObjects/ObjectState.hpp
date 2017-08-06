#pragma once
using scalar = float;
struct vector {
	scalar h;
	scalar v;

	vector(scalar h, scalar v) : h(h), v(v) {}
	inline vector& operator+=(vector const& p) {
		h += p.h;
		v += p.v;
		return *this;
	}
	inline vector& operator*=(scalar const m) {
		h *= m;
		v *= m;
		return *this;
	}
	inline vector& operator*=(vector const& m) {
		h *= m.h;
		v *= m.v;
		return *this;
	}
	friend vector const operator+(vector const& a, vector const& b);
	friend vector const operator-(vector const& a, vector const& b);
	friend vector const operator*(vector const& a, scalar b);
	friend vector const operator*(vector const& a, vector const& b);
};

class DependentObjectState {
protected:
	scalar m_mass;
	vector m_acceleration;
public:
	DependentObjectState(scalar mass, scalar m_acceleration_h = 0.f, scalar m_acceleration_v = 0.f)
		: m_mass(mass), m_acceleration(m_acceleration_h, m_acceleration_v) {}

	virtual scalar mass() const {
		return m_mass;
	}
	virtual void addMass(scalar const& mass) {
		m_mass += mass;
	}
	virtual void mulMass(scalar const& mass) {
		m_mass *= mass;
	}

	virtual vector acceleration() const {
		return m_acceleration;
	}
	virtual void accelerate(vector const& difference) {
		m_acceleration += difference;
	}
	virtual void stopAcceleration_h() {
		m_acceleration.h = 0.f;
	}
	virtual void stopAcceleration_v() {
		m_acceleration.v = 0.f;
	}
};

class IndependentObjectState : public DependentObjectState {
protected:
	vector m_size;
	vector m_position;
	vector m_speed;
public:
	IndependentObjectState(scalar mass, scalar size_h, scalar size_v,
						   scalar position_h = 0.f, scalar position_v = 0.f,
						   scalar speed_h = 0.f, scalar speed_v = 0.f,
						   scalar m_acceleration_h = 0.f, scalar m_acceleration_v = 0.f)
		: DependentObjectState(mass, m_acceleration_h, m_acceleration_v), 
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