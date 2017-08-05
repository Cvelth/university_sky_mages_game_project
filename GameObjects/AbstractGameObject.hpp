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
	friend vector const operator+(vector const& a, vector const& b);
	friend vector const operator-(vector const& a, vector const& b);
	friend vector const operator*(vector const& a, scalar b);
};

struct ObjectState {
	float mass;
	vector size;
	vector position;
	vector speed;
	vector acceleration;

	ObjectState(float mass, float size_h, float size_v,
				float position_h = 0.f, float position_v = 0.f,
				float speed_h = 0.f, float speed_v = 0.f,
				float m_acceleration_h = 0.f, float m_acceleration_v = 0.f)
		: mass(mass), size(size_h, size_v), position(position_h, position_v),
		speed(speed_h, speed_v), acceleration(m_acceleration_h, m_acceleration_v) {}
};

class AbstractObjectQueue;
class RenderInfo;
class PhysicsEngine;

class AbstractGameObject {
	friend AbstractObjectQueue;
	friend PhysicsEngine;
protected:
	ObjectState m_state;
	RenderInfo* m_render_info;

	size_t m_queue_counter;

	bool m_waiting_to_be_initilized;
	bool m_waiting_to_be_destroyed;
protected:
	inline void wasAddedToQueue() {
		m_queue_counter++;
	}
	inline void wasRemovedFromQueue() {
		if (m_queue_counter--)
			delete this;
	}
	inline const size_t getQueuesCount() const {
		return m_queue_counter;
	}
public:
	AbstractGameObject(RenderInfo* render_info, float mass, float size_h, 
					   float size_v, float position_h, float position_v) 
		: m_render_info(render_info), m_state(mass, size_h, size_v, position_h, position_v), 
		m_waiting_to_be_initilized(true), m_waiting_to_be_destroyed(false) {}
	virtual ~AbstractGameObject() {}

	inline bool isWaitingToBeInitilized() const {
		return m_waiting_to_be_initilized;
	}
	inline void initializeWasSuccessfull() {
		m_waiting_to_be_initilized = false;
	}
	inline void reinitialize() {
		m_waiting_to_be_initilized = true;
	}
	inline bool isWaitingToBeDestroyed() const {
		return m_waiting_to_be_destroyed;
	}
	inline void destructionWasSuccessfull() {
		m_waiting_to_be_destroyed = false;
	}
	inline void destroy() {
		m_waiting_to_be_destroyed = true;
	}

	inline RenderInfo* getRenderInto() {
		return m_render_info;
	}

	inline const vector& position() const {
		return m_state.position;
	}
};
