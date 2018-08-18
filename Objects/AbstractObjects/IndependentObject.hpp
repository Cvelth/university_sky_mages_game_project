#pragma once
#include <string>
#include <memory>
#include "Objects/ObjectState/IndependentObject.hpp"
class PhysicsEngine;
class RenderInfo;

class IndependentObject : public IndependentObjectState {
	friend PhysicsEngine;
protected:
	std::shared_ptr<RenderInfo> m_render_info;

	bool m_isInitialized;
public:
	IndependentObject(std::shared_ptr<RenderInfo> render_info, float mass, float size_h,
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

	inline std::shared_ptr<RenderInfo> getRenderInto() {
		return m_render_info;
	}

	using IndependentObjectState::acceleration;
	using IndependentObjectState::speed;
	using IndependentObjectState::position;

	virtual operator std::string() {
		return std::string(); //to be implemented.
	}
};