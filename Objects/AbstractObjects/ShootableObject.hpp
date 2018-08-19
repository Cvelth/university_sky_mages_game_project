#pragma once
#include "Objects/AbstractObjects/IndependentObject.hpp"

class ShootableObject : public IndependentObject {
	friend PhysicsEngine;
protected:
	float m_damage;
public:
	ShootableObject(float damage, std::shared_ptr<RenderInfo> render_info, float mass, 
					float size_h, float size_v, float position_h, float position_v, 
					float destination_h, float destination_v, float m_initial_ammo_speed)
					: IndependentObject(render_info, mass, size_h, size_v, position_h, position_v), m_damage(damage) {
		speed_up(mgl::math::vector(destination_h - position_h, destination_v - position_v).normalized() * m_initial_ammo_speed);
	}
	ShootableObject(std::shared_ptr<RenderInfo> render_info, float mass, 
					vector acceleration, vector speed, vector position, 
					vector size, float damage) : IndependentObject(render_info, mass, size.at(0), size.at(1), 
																   position.at(0), position.at(1)), m_damage(damage) {
		accelerate(acceleration);
		speed_up(speed);
	}
	float damage() const { return m_damage; }
};