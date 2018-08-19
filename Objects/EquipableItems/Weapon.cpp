#include "Weapon.hpp"
#include "Objects/AbstractObjects/ShootableObject.hpp"
#include "Objects/EquipableItems/EnergyStorage.hpp"
#include "Engines/ObjectStorage/RenderInfoStorage.hpp"

std::shared_ptr<RenderInfo> projectileRenderInfo(AmmoProjectileType ammo_type) {
	switch (ammo_type) {
	case AmmoProjectileType::Bullet: return RenderInfoStorage::getRenderInfo("AbstractBulletProjectile");
	case AmmoProjectileType::Physical: return RenderInfoStorage::getRenderInfo("AbstractPhysicalProjectile");
	case AmmoProjectileType::Energy: return RenderInfoStorage::getRenderInfo("AbstractEnergyProjectile");
	default: throw Exceptions::UnsupportedProjectileType("Unsupported projectile type was attempted to be rendered.");
	}
}
#include "TimeControl.hpp"
bool Weapon::is_reloaded() const {
	if (m_current_ammo)
		return now() > Duration(m_last_shot_time + microseconds(1.e6f / m_firerate)).count();
	else
		if (now() > Duration(m_last_shot_time + microseconds(1.e6f * m_reload_cooldown)).count()) {
			m_current_ammo = m_ammo_capacity;
			return true;
		}
		else
			return false;
}
std::shared_ptr<ShootableObject> Weapon::shoot(float current_x, float current_y, float destination_x, float destination_y) const {
	if (is_ready() && !m_energy_source->isEmpty() && m_energy_source->use(m_initial_ammo_speed * m_firerate * m_initial_ammo_mass / m_energy_usage_coefficient)) {
		m_current_ammo--;
		m_last_shot_time = now();
		if (!m_autofire_supported)
			is_activated = false;
		return std::make_shared<ShootableObject>(m_damage, projectileRenderInfo(m_ammo_type),
			m_initial_ammo_mass, m_initial_ammo_size_h, m_initial_ammo_size_v,
			current_x, current_y, destination_x, destination_y, m_initial_ammo_speed);
	} else
		return nullptr;
}