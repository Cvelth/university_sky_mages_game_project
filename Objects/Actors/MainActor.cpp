#include "MainActor.hpp"
#include "Objects/EquipableItems/EnergyStorage.hpp"
#include "Objects/EquipableItems/FlyEngine.hpp"
#include "Objects/EquipableItems/Weapon.hpp"

void MainActor::giveEnergyStorage(EnergyStorage *es) {
	m_energy_storage = es;
	if (m_engine) m_engine->connect_to_energy_source(m_energy_storage);
	if (m_weapon_left_arm) m_weapon_left_arm->connect_to_energy_source(m_energy_storage);
	if (m_weapon_right_arm) m_weapon_right_arm->connect_to_energy_source(m_energy_storage);
}

void MainActor::giveFlyEngine(FlyEngine *fe) {
	if (m_engine) m_engine->connect_to_energy_source(nullptr);
	m_engine = fe;
	fe->connect_to_energy_source(m_energy_storage);
}

void MainActor::giveLeftWeapon(Weapon *w) {
	switch (w->size()) {
		case WeaponSize::One_Arm:
		case WeaponSize::One_And_A_Half_Arm:
		case WeaponSize::Small:
			if (m_weapon_right_arm &&
				(m_weapon_right_arm->size() == WeaponSize::Two_Arm ||
				 m_weapon_right_arm->size() == WeaponSize::Big)) {

				m_weapon_right_arm->connect_to_energy_source(nullptr);
				m_weapon_right_arm = nullptr;
			}

			if (m_weapon_left_arm) m_weapon_left_arm->connect_to_energy_source(nullptr);
			m_weapon_left_arm = w;
			m_weapon_left_arm->connect_to_energy_source(m_energy_storage);
			break;
		case WeaponSize::Two_Arm:
		case WeaponSize::Big:
			if (m_weapon_left_arm) m_weapon_left_arm->connect_to_energy_source(nullptr);
			if (m_weapon_right_arm) m_weapon_right_arm->connect_to_energy_source(nullptr);
			m_weapon_left_arm = w;
			m_weapon_right_arm = w;
			m_weapon_right_arm->connect_to_energy_source(m_energy_storage);
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

			m_weapon_left_arm->connect_to_energy_source(nullptr);
			m_weapon_left_arm = nullptr;
		}

		if (m_weapon_right_arm) m_weapon_right_arm->connect_to_energy_source(nullptr);
		m_weapon_right_arm = w;
		m_weapon_right_arm->connect_to_energy_source(m_energy_storage);
		break;
	case WeaponSize::Two_Arm:
	case WeaponSize::Big:
		if (m_weapon_left_arm) m_weapon_left_arm->connect_to_energy_source(nullptr);
		if (m_weapon_right_arm) m_weapon_right_arm->connect_to_energy_source(nullptr);
		m_weapon_left_arm = w;
		m_weapon_right_arm = w;
		m_weapon_right_arm->connect_to_energy_source(m_energy_storage);
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
	return { shootRightWeapon(), shootLeftWeapon() };
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

MainActor::MainActor(float mass, mgl::math::vector const& acceleration, mgl::math::vector const& speed, mgl::math::vector const& position, mgl::math::vector const& size, std::shared_ptr<RenderInfo> render_info) : Actor(render_info, mass, size.at(0), size.at(1), position.at(0), position.at(1)) {
	accelerate(acceleration);
	speed_up(speed);
}
MainActor::~MainActor() {
	if (m_energy_storage) delete m_energy_storage;
	if (m_engine) delete m_engine;
	if (m_weapon_left_arm) delete m_weapon_left_arm;
	if (m_weapon_right_arm) delete m_weapon_right_arm;
}

#include <sstream>
#include "../../Engines/ObjectStorage/RenderInfoStorage.hpp"
MainActor::operator std::string() const {
	std::ostringstream s;
	s << "MainActor " << m_mass << ' ' << m_acceleration.at(0) << ' ' << m_acceleration.at(1) << ' '
		<< m_speed.at(0) << ' ' << m_speed.at(1) << ' ' << m_position.at(0) << ' ' << m_position.at(0) << ' '
		<< m_size.at(0) << ' ' << m_size.at(1) << ' ' << RenderInfoStorage::getRenderInfo(m_render_info) << ' '
		<< (m_energy_storage ? m_energy_storage->name() : "NO") << ' '
		<< (m_engine ? m_engine->name() : "NO") << ' '
		<< (m_weapon_left_arm ? m_weapon_left_arm->name() : "NO") << ' '
		<< (m_weapon_right_arm ? m_weapon_right_arm->name() : "NO") << ' ';
	return s.str();
}