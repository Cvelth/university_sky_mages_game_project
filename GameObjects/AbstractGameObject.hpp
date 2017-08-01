#pragma once

struct ObjectState {
	float m_position_h;
	float m_position_v;
	float m_speed_h;
	float m_speed_v;
	float m_acceleration_h;
	float m_acceleration_v;

	ObjectState(float position_h = 0.f, float position_v = 0.f,
				float speed_h = 0.f, float speed_v = 0.f,
				float m_acceleration_h = 0.f, float m_acceleration_v = 0.f)
						: m_position_h(position_h), m_position_v(position_v), 
						m_speed_h(speed_h), m_speed_v(speed_v),
						m_acceleration_h(m_acceleration_h), m_acceleration_v(m_acceleration_v) {
	
	}
};

class RenderInfo;

class AbstractGameObject {
protected:
	ObjectState m_state;
	RenderInfo* m_render_info;

	bool m_waiting_to_be_initilized;
	bool m_waiting_to_be_destroyed;
public:
	AbstractGameObject(RenderInfo* render_info, float position_h, float position_v) 
		: m_render_info(render_info), m_state(position_h, position_v), 
		m_waiting_to_be_initilized(true), m_waiting_to_be_destroyed(false) {}
	virtual ~AbstractGameObject();

	inline bool isWaitingToBeInitilized() {
		return m_waiting_to_be_initilized;
	}
	inline void initializeWasSuccessfull() {
		m_waiting_to_be_initilized = false;
	}
	inline void reinitialize() {
		m_waiting_to_be_initilized = true;
	}
	inline bool isWaitingToBeDestroyed() {
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
};
