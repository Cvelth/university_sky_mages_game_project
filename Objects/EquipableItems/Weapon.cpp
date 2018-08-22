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
#include "Shared/TimeControl.hpp"
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

template<>
inline void Weapon::set_value<std::string>(std::string const& name, std::string const& value) {
	if (name == "name")
		m_name = value;
	else if (name == "description")
		m_description = value;
	else
		throw Exceptions::UnsupportedValueException("Unsupported value was passed");
}
template<>
inline void Weapon::set_value<unsigned int>(std::string const& name, unsigned int const& value) {
	if (name == "ammo_type")
		m_ammo_type = static_cast<AmmoProjectileType>(value);
	else if (name == "size")
		m_size = static_cast<WeaponSize>(value);
	else if (name == "ammo_capacity") {
		m_ammo_capacity = value;
		m_current_ammo = value;
	} else
		throw Exceptions::UnsupportedValueException("Unsupported value was passed");
}
template<>
inline void Weapon::set_value<bool>(std::string const& name, bool const& value) {
	if (name == "autofire_support")
		m_autofire_supported = value;
	else
		throw Exceptions::UnsupportedValueException("Unsupported value was passed");
}
template<>
inline void Weapon::set_value<float>(std::string const& name, float const& value) {
	if (name == "mass")
		addMass(value);
	else if (name == "energy_usage_coefficient")
		m_energy_usage_coefficient = value;
	else if (name == "damage")
		m_damage = value;
	else if (name == "firerate")
		m_firerate = value;
	else if (name == "initial_ammo_speed")
		m_initial_ammo_speed = value;
	else if (name == "initial_ammo_mass")
		m_initial_ammo_mass = value;
	else if (name == "initial_ammo_size_h")
		m_initial_ammo_size_h = value;
	else if (name == "initial_ammo_size_v")
		m_initial_ammo_size_v = value;
	else if (name == "reload_cooldown")
		m_reload_cooldown = value;
	else
		throw Exceptions::UnsupportedValueException("Unsupported value was passed");
}