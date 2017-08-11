#include "AbstractEquipableItems.hpp"
#include "AbstractGameObject.hpp"
#include "RenderTools\RenderInfoStorage.hpp"
#include <chrono>
using microseconds = unsigned long long;
using Clock = std::chrono::high_resolution_clock;
using Point = std::chrono::time_point<std::chrono::high_resolution_clock>;
using Duration = std::chrono::duration<microseconds>;

bool AbstractWeapon::is_reloaded() const {
	auto fire_wait = 1.e6f / m_fire_rate;
	return Clock::now() > Point(Duration(m_last_shot_time + (microseconds)fire_wait));
}

AbstractShootableObject* AbstractWeapon::shoot(float current_x, float current_y, float destination_x, float destination_y) {
	if (is_ready()) {
		m_last_shot_time = std::chrono::duration_cast<std::chrono::microseconds>(Clock::now().time_since_epoch()).count();
		return new AbstractShootableObject(m_damage, RenderInfoStorage::getAbstractBulletRenderInfo(), m_ammo_mass, m_ammo_size_h, m_ammo_size_v, current_x, current_y);
	}
}
