#pragma once
#include "Objects/ObjectState/IndependentObject.hpp"
class TheQueue;
class PhysicsEngine;
class RenderInfo;

class IndependentObject : protected IndependentObjectState {
	friend TheQueue;
	friend PhysicsEngine;
protected:
	RenderInfo* m_render_info;

	bool m_isInitialized;
public:
	IndependentObject(RenderInfo* render_info, float mass, float size_h,
					   float size_v, float position_h, float position_v)
		: IndependentObjectState(mass, size_h, size_v, position_h, position_v), m_render_info(render_info),
		m_isInitialized(false) {}
		
	virtual ~IndependentObject() {}

	inline bool isInitialized() const {
		return m_isInitialized;
	}
	inline void initializationWasSuccessfull() {
		m_isInitialized = true;
	}
	inline void reinitialize() {
		m_isInitialized = false;
	}

	inline RenderInfo* getRenderInto() {
		return m_render_info;
	}

	using IndependentObjectState::position;
};