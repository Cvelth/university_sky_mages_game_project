#pragma once
#include "Objects\AbstractObjects\IndependentObject.hpp"

class ShootableObject : public IndependentObject {
	friend TheQueue;
	friend PhysicsEngine;
protected:
	float m_damage;
public:
	ShootableObject(float damage, RenderInfo* render_info, float mass, float size_h,
					float size_v, float position_h, float position_v)
		: IndependentObject(render_info, mass, size_h, size_v, position_h, position_v), m_damage(damage) {}
};