#include "MainActor.hpp"
#include "Objects/EquipableItems/EnergyStorage.hpp"
#include "Objects/EquipableItems/FlyEngine.hpp"
#include "Objects/EquipableItems/Weapon.hpp"
#include "Objects/EquipableItems/Shield.hpp"
#include "Objects/EquipableItems/Trinket.hpp"

void MainActor::giveEnergyStorage(EnergyStorage *es) {
	m_energy_storage = es;
	if (m_engine) m_engine->connect_to_energy_source(m_energy_storage);
	if (m_weapon_left_arm) m_weapon_left_arm->connect_to_energy_source(m_energy_storage);
	if (m_weapon_right_arm) m_weapon_right_arm->connect_to_energy_source(m_energy_storage);
	if (m_shield) m_shield->connect_to_energy_source(m_energy_storage);
}
void MainActor::giveFlyEngine(FlyEngine *fe) {
	if (m_engine) m_engine->connect_to_energy_source(nullptr);
	m_engine = fe;
	if (m_engine) m_engine->connect_to_energy_source(m_energy_storage);
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
			if (m_weapon_right_arm) m_weapon_right_arm->connect_to_energy_source(m_energy_storage);
			break;
		case WeaponSize::Two_Arm:
		case WeaponSize::Big:
			if (m_weapon_left_arm) m_weapon_left_arm->connect_to_energy_source(nullptr);
			if (m_weapon_right_arm) m_weapon_right_arm->connect_to_energy_source(nullptr);
			m_weapon_left_arm = w;
			m_weapon_right_arm = w;
			if (m_weapon_right_arm) m_weapon_right_arm->connect_to_energy_source(m_energy_storage);
			break;
		default:
			throw Exceptions::UnsupportableItemWasGivenException("This weapon cannot be equiped.");
	}
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
			if (m_weapon_left_arm) m_weapon_left_arm->connect_to_energy_source(m_energy_storage);
			break;
		case WeaponSize::Two_Arm:
		case WeaponSize::Big:
			if (m_weapon_left_arm) m_weapon_left_arm->connect_to_energy_source(nullptr);
			if (m_weapon_right_arm) m_weapon_right_arm->connect_to_energy_source(nullptr);
			m_weapon_left_arm = w;
			m_weapon_right_arm = w;
			if (m_weapon_right_arm) m_weapon_right_arm->connect_to_energy_source(m_energy_storage);
			break;
		default:
			throw Exceptions::UnsupportableItemWasGivenException("This weapon cannot be equiped.");
	}
}
void MainActor::giveShieldGenerator(ShieldGenerator *sg) {
	if (m_shield) m_shield->connect_to_energy_source(nullptr);
	m_shield = sg;
	if (m_shield) m_shield->connect_to_energy_source(m_energy_storage);
}
void MainActor::giveTrinket(Trinket *t) {
	m_trinket = t;
}

EnergyStorage* MainActor::takeEnergyStorage() {
	if (m_engine) m_engine->connect_to_energy_source(nullptr);
	if (m_weapon_left_arm) m_weapon_left_arm->connect_to_energy_source(nullptr);
	if (m_weapon_right_arm) m_weapon_right_arm->connect_to_energy_source(nullptr);
	if (m_shield) m_shield->connect_to_energy_source(nullptr);
	auto ret = m_energy_storage;
	m_energy_storage = nullptr;
	return ret;
}
FlyEngine* MainActor::takeFlyEngine() {
	if (m_engine) m_engine->connect_to_energy_source(nullptr);
	auto ret = m_engine;
	m_engine = nullptr;
	return ret;
}
Weapon* MainActor::takeRightWeapon() {
	m_weapon_right_arm->connect_to_energy_source(nullptr);
	auto ret = m_weapon_right_arm;
	m_weapon_right_arm = nullptr;
	return ret;
}
Weapon* MainActor::takeLeftWeapon() {
	m_weapon_left_arm->connect_to_energy_source(nullptr);
	auto ret = m_weapon_left_arm;
	m_weapon_left_arm = nullptr;
	return ret;
}
ShieldGenerator* MainActor::takeShieldGenerator() {
	m_shield->connect_to_energy_source(nullptr);
	auto ret = m_shield;
	m_shield = nullptr;
	return ret;
}
Trinket* MainActor::takeTrinket() {
	auto ret = m_trinket;
	m_trinket = nullptr;
	return ret;
}

void MainActor::aim(float x, float y) {
	m_aim_x = x;
	m_aim_y = y;
}
void MainActor::activateRightWeapon() {
	if (m_weapon_right_arm) m_weapon_right_arm->activate();
}
void MainActor::deactivateRightWeapon() {
	if (m_weapon_right_arm) m_weapon_right_arm->deactivate();
}
void MainActor::activateLeftWeapon() {
	if (m_weapon_left_arm) m_weapon_left_arm->activate();
}
void MainActor::deactivateLeftWeapon() {
	if (m_weapon_left_arm) m_weapon_left_arm->deactivate();
}
void MainActor::activateShieldGenerator() {
	if (m_shield) m_shield->activate();
}
void MainActor::deactivateShieldGenerator() {
	if (m_shield) m_shield->deactivate();
}

std::shared_ptr<ShootableObject> MainActor::shootRightWeapon(size_t id) {
	if (m_weapon_right_arm)
		return m_weapon_right_arm->shoot(id, m_position[0], m_position[1], m_aim_x, m_aim_y);
	return nullptr;
}
std::shared_ptr<ShootableObject> MainActor::shootLeftWeapon(size_t id) {
	if (m_weapon_left_arm && m_weapon_right_arm != m_weapon_left_arm)
		return m_weapon_left_arm->shoot(id, m_position[0], m_position[1], m_aim_x, m_aim_y);
	return nullptr;
}
std::vector<std::shared_ptr<ShootableObject>> MainActor::shootingProcess(size_t id) {
	if (m_shield) m_shield->shield();
	return { shootRightWeapon(id), shootLeftWeapon(id) };
}

vector MainActor::acceleration(scalar const& time_correct) const {
	return m_acceleration +
		(m_engine ? m_engine->acceleration(mass(), time_correct) : vector(0.f, 0.f));
}
vector MainActor::get_acceleration() const {
	if (m_engine)
		return m_acceleration + m_engine->get_acceleration();
	else
		return m_acceleration;
}
void MainActor::update_state(vector const& acceleration, vector const& speed, vector const& position, float capacity_percent) {
	m_engine->update_acceleration(acceleration);
	m_speed = speed;
	m_position = position;
	if (m_energy_storage)
		m_energy_storage->update(capacity_percent);
}
scalar MainActor::mass() const {
	return m_mass + 
		(m_engine ? m_engine->mass() : 0.f) + 
		(m_energy_storage ? m_energy_storage->mass() : 0.f);
}

MainActor::MainActor(float mass, vector const& acceleration, vector const& speed, vector const& position, vector const& size, std::string const& render_info)
	: Actor(render_info, mass, size.at(0), size.at(1), position.at(0), position.at(1)), m_energy_storage(nullptr), m_engine(nullptr), m_weapon_left_arm(nullptr), m_weapon_right_arm(nullptr), m_shield(nullptr), m_trinket(nullptr) {
	m_acceleration = acceleration;
	m_speed = speed;
}
MainActor::~MainActor() {
	if (m_energy_storage) delete m_energy_storage;
	if (m_engine) delete m_engine;
	if (m_weapon_left_arm) delete m_weapon_left_arm;
	if (m_weapon_right_arm) delete m_weapon_right_arm;
	if (m_shield) delete m_shield;
	if (m_trinket) delete m_trinket;
}

#include <iostream>
#include <random>
#include "Objects/EquipableItems/Trinket.hpp"
static std::mt19937 g((std::random_device())());
static std::uniform_real_distribution<float> d(0.f, 1.f);
void MainActor::was_hit(std::shared_ptr<ShootableObject> so) {
	if (!m_shield || !m_shield->was_hit(so)) {
		float damage = d(g);
		if (damage <= 0) {
			std::cout << "nothing.\n: ";
			return;
		}
		if (m_trinket) {
			damage -= m_trinket->chance_to_take_damage();
			if (damage <= 0) {
				std::cout << m_trinket->name() << ".\n: ";
				delete takeTrinket();
				return;
			}
		}
		if (m_shield) {
			damage -= m_shield->chance_to_take_damage();
			if (damage <= 0) {
				std::cout << m_shield->name() << ".\n: ";
				delete takeShieldGenerator();
				return;
			}
		}
		if (m_weapon_right_arm) {
			damage -= m_weapon_right_arm->chance_to_take_damage();
			if (damage <= 0) {
				std::cout << m_weapon_right_arm->name() << ".\n: ";
				delete takeRightWeapon();
				return;
			}
		}
		if (m_weapon_left_arm) {
			damage -= m_weapon_left_arm->chance_to_take_damage();
			if (damage <= 0) {
				std::cout << m_weapon_left_arm->name() << ".\n: ";
				delete takeLeftWeapon();
				return;
			}
		}
		if (m_engine) {
			damage -= m_engine->chance_to_take_damage();
			if (damage <= 0) {
				std::cout << m_engine->name() << ".\n: ";
				delete takeFlyEngine();
				return;
			}
		}
		if (m_energy_storage) {
			damage -= m_energy_storage->chance_to_take_damage();
			if (damage <= 0) {
				std::cout << m_energy_storage->name() << ".\n: ";
				delete takeEnergyStorage();
				return;
			}
		}
		std::cout << "his life.\n: ";
		die();
	}
}