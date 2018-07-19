#include "MainActor.hpp"
#include "Objects/EquipableItems/EnergyStorage.hpp"
#include "Objects/EquipableItems/FlyEngine.hpp"
#include "Objects/EquipableItems/Weapon.hpp"

void MainActor::giveLeftWeapon(Weapon *w) {
	switch (w->size()) {
		case WeaponSize::One_Arm:
		case WeaponSize::One_And_A_Half_Arm:
		case WeaponSize::Small:
			if (m_weapon_right_arm &&
				(m_weapon_right_arm->size() == WeaponSize::Two_Arm ||
				 m_weapon_right_arm->size() == WeaponSize::Big)) {

				m_weapon_right_arm = nullptr;
			}
			m_weapon_left_arm = w;
			break;
		case WeaponSize::Two_Arm:
		case WeaponSize::Big:
			m_weapon_left_arm = w;
			m_weapon_right_arm = w;
			break;
		default:
			throw Exceptions::UnsupportableItemWasGivenException("This weapon cannot be equiped.");
	}
}
void MainActor::giveRightWeapon(Weapon *w) {
	switch (w->size()) {
		case WeaponSize::One_Arm:
		case WeaponSize::One_And_A_Half_Arm:
		case WeaponSize::Small:
			if (m_weapon_left_arm &&
				(m_weapon_left_arm->size() == WeaponSize::Two_Arm ||
				 m_weapon_left_arm->size() == WeaponSize::Big)) {

				m_weapon_left_arm = nullptr;
			}
			m_weapon_right_arm = w;
			break;
		case WeaponSize::Two_Arm:
		case WeaponSize::Big:
			m_weapon_left_arm = w;
			m_weapon_right_arm = w;
			break;
		default:
			throw Exceptions::UnsupportableItemWasGivenException("This weapon cannot be equiped.");
	}
}

void MainActor::activateRightWeapon() {
	if (m_weapon_right_arm) m_weapon_right_arm->activate();
}
void MainActor::activateLeftWeapon() {
	if (m_weapon_left_arm) m_weapon_left_arm->activate();
}
void MainActor::aim(float x, float y) {
	m_aim_x = x;
	m_aim_y = y;
}
void MainActor::deactivateRightWeapon() {
	if (m_weapon_right_arm) m_weapon_right_arm->deactivate();
}
void MainActor::deactivateLeftWeapon() {
	if (m_weapon_left_arm) m_weapon_left_arm->deactivate();
}

ShootableObject* MainActor::shootRightWeapon() {
	if (m_weapon_right_arm)
		return m_weapon_right_arm->shoot(m_position[0], m_position[1], m_aim_x, m_aim_y);
	return nullptr;
}
ShootableObject* MainActor::shootLeftWeapon() {
	if (m_weapon_left_arm && m_weapon_right_arm != m_weapon_left_arm)
		return m_weapon_left_arm->shoot(m_position[0], m_position[1], m_aim_x, m_aim_y);
	return nullptr;
}
std::vector<ShootableObject*> MainActor::shootingProcess() {
	std::vector<ShootableObject*> ret;
	ret.push_back(shootRightWeapon());
	ret.push_back(shootLeftWeapon());
	return ret;
}

vector MainActor::acceleration(scalar const& time_correct) const {
	return m_acceleration +
		(m_engine ? m_engine->acceleration(mass(), time_correct) : vector(0.f, 0.f));
}
scalar MainActor::mass() const {
	return m_mass + 
		(m_engine ? m_engine->mass() : 0.f) + 
		(m_energy_storage ? m_energy_storage->mass() : 0.f);
}
MainActor::~MainActor() {
	if (m_energy_storage) delete m_energy_storage;
	if (m_engine) delete m_engine;
	if (m_weapon_left_arm)delete m_weapon_left_arm;
	if (m_weapon_right_arm)delete m_weapon_right_arm;
}