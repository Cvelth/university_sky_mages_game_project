#include "Weapon.hpp"
#include "Objects/AbstractObjects/ShootableObject.hpp"
#include "Engines/RenderTools/RenderInfoStorage.hpp"

#include <chrono>
using microseconds = unsigned long long;
using Clock = std::chrono::high_resolution_clock;
using Point = std::chrono::time_point<std::chrono::high_resolution_clock>;
using Duration = std::chrono::duration<microseconds>;

bool Weapon::is_reloaded() const {
	if (m_current_ammo)
		return Clock::now() > Point(Duration(m_last_shot_time + microseconds(1.e6f / m_firerate)));
	else
		if (Clock::now() > Point(Duration(m_last_shot_time + microseconds(1.e6f * m_reload_cooldown)))) {
			m_current_ammo = m_ammo_capacity;
			return true;
		} else
			return false;
}

ShootableObject* Weapon::shoot(float current_x, float current_y, float destination_x, float destination_y) const {
	if (is_ready()) {
		m_current_ammo--;
		m_last_shot_time = std::chrono::duration_cast<std::chrono::microseconds>(Clock::now().time_since_epoch()).count();
		return new ShootableObject(m_damage, RenderInfoStorage::getAbstractBulletRenderInfo(), 
			m_initial_ammo_mass, m_initial_ammo_size_h, m_initial_ammo_size_v, current_x, current_y);
	} else
		return nullptr;
}