#pragma once
#include "Objects/Objects/IndependentObject.hpp"

enum class ShootableObjectType {
	Energy = 1, Physical = 2, Bullet = 3
};
class ShootableObject : public IndependentObject {
	friend PhysicsEngine;
protected:
	ShootableObjectType m_type;
	size_t m_shooter_id;
	float m_damage;
public:
	ShootableObject(ShootableObjectType type, size_t shooter_id, float damage, std::string const& render_info, float mass,
					float size_h, float size_v, float position_h, float position_v, 
					float destination_h, float destination_v, float m_initial_ammo_speed)
					: IndependentObject(render_info, mass, size_h, size_v, position_h, position_v), m_damage(damage), m_type(type), m_shooter_id(shooter_id) {
		speed_up(mgl::math::vector(destination_h - position_h, destination_v - position_v).normalized() * m_initial_ammo_speed);
	}
	ShootableObject(ShootableObjectType type, size_t shooter_id, std::string const& render_info, float mass,
					vector acceleration, vector speed, vector position,
					vector size, float damage) : IndependentObject(render_info, mass, size.at(0), size.at(1),
																   position.at(0), position.at(1)), m_damage(damage),
												 m_type(type), m_shooter_id(shooter_id) {
		accelerate(acceleration);
		speed_up(speed);
	}
	ShootableObjectType type() const { return m_type; }
	size_t shooter_id() const { return m_shooter_id; }
	float damage() const { return m_damage; }
	float impulse() const { return m_speed.length() * m_mass; }
};

#include "Shared/AbstractException.hpp"
DefineNewException(UnsupportedProjectileType);