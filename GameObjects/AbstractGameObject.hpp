#pragma once
struct vector;

class AbstractObjectQueue;
class RenderInfo;
class PhysicsEngine;
class IndependentObjectState;

class AbstractGameObject {
	friend AbstractObjectQueue;
	friend PhysicsEngine;
protected:
	IndependentObjectState* m_state;
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
					   float size_v, float position_h, float position_v);
		
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

	vector position() const;
};
