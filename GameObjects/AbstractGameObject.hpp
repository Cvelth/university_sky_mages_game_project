#pragma once

struct Vector {
	float h;
	float v;

	Vector(float h, float v) : h(h), v(v) {}
	inline Vector& operator+=(Vector& p) {
		h += p.h;
		v += p.v;
		return *this;
	}
};

struct ObjectState {
	float mass;
	Vector position;
	Vector speed;
	Vector acceleration;

	ObjectState(float mass, 
				float position_h = 0.f, float position_v = 0.f,
				float speed_h = 0.f, float speed_v = 0.f,
				float m_acceleration_h = 0.f, float m_acceleration_v = 0.f)
						: mass(mass), position(position_h, position_v), speed(speed_h, speed_v),
						acceleration(m_acceleration_h, m_acceleration_v) {
	
	}
};

class AbstractObjectQueue;
class RenderInfo;

class AbstractGameObject {
	friend AbstractObjectQueue;
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
	AbstractGameObject(RenderInfo* render_info, float mass, float position_h, float position_v) 
		: m_render_info(render_info), m_state(mass, position_h, position_v), 
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

	inline const Vector& position() const {
		return m_state.position;
	}
	inline const float mass() const {
		return m_state.mass;
	}

	inline void accelerate(float h, float v) {
		m_state.acceleration += Vector(h, v);
	}
	inline void updateSpeed() {
		m_state.speed += m_state.acceleration;
	}
	inline void updatePosition() {
		m_state.position += m_state.speed;
	}
};
