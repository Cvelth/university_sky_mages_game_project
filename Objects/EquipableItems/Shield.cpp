#include "Shield.hpp"
#include "../AbstractObjects/ShootableObject.hpp"
float ShieldGenerator::get_efficience_coefficient(ShootableObjectType type) {
	switch (type) {
		case ShootableObjectType::Energy:
			return 1.f - m_anti_energy_efficiency;
		case ShootableObjectType::Physical:
			return 1.f - m_anti_projectile_efficiency;
		case ShootableObjectType::Bullet:
			return 1.f - m_anti_bullet_efficiency;
		default:
			throw Exceptions::UnsupportedProjectileType("Unsupported projectile has hit a shield.");
	}
}

#include "../EquipableItems/EnergyStorage.hpp"
void ShieldGenerator::activate() {
	if (m_energy_source && m_energy_source->use(m_activation_energy))
		is_activated = true;
}
bool ShieldGenerator::was_hit(ShootableObject *so) {
	if (!is_activated || !m_energy_source || m_energy_source->isEmpty())
		return false;
	return m_energy_source->use(so->damage() * so->impulse() * get_efficience_coefficient(so->type()));
}

#include "Shared/TimeControl.hpp"
ShieldGenerator::ShieldGenerator() : EquipableItem(), m_energy_source(nullptr), is_activated(false) {
	m_energy_usage_time = now();
}
void ShieldGenerator::shield() const {
	auto time = now();
	if (is_activated && (!m_energy_source || m_energy_source->isEmpty() || !m_energy_source->use(m_energy_per_second * float(time - m_energy_usage_time) / 1.e6f)))
		is_activated = false;
}