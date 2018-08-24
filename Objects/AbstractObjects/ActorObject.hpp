#pragma once
#include "Objects/AbstractObjects/IndependentObject.hpp"

class Actor : public IndependentObject {
protected:
	bool m_is_alive;
public:
	Actor(std::string const& render_info, float mass, float size_h,
		  float size_v, float position_h, float position_v) 
		: IndependentObject(render_info, mass, size_h, size_v, position_h, position_v), m_is_alive(true) {}
	~Actor() {}

	bool is_alive() const { return m_is_alive; }
	void die() { m_is_alive = false; }
};