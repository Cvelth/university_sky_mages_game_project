#include "MainActor.hpp"
#include "Objects/EquipableItems/EnergyStorage.hpp"
#include "Objects/EquipableItems/FlyEngine.hpp"
#include "Objects/EquipableItems/Weapon.hpp"
#include "Objects/EquipableItems/Shield.hpp"
#include "Objects/EquipableItems/Trinket.hpp"

void MainActor::giveEnergyStorage(std::shared_ptr<EnergyStorage> es) {
	m_energy_storage = es;
	if (m_engine) m_engine->connect_to_energy_source(m_energy_storage);
	if (m_weapon_left_arm) m_weapon_left_arm->connect_to_energy_source(m_energy_storage);
	if (m_weapon_right_arm) m_weapon_right_arm->connect_to_energy_source(m_energy_storage);
	if (m_shield) m_shield->connect_to_energy_source(m_energy_storage);
}
void MainActor::giveFlyEngine(std::shared_ptr<FlyEngine> fe) {
	if (m_engine) m_engine->connect_to_energy_source(nullptr);
	m_engine = fe;
	if (m_engine) m_engine->connect_to_energy_source(m_energy_storage);
}
void MainActor::giveRightWeapon(std::shared_ptr<Weapon> w) {
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
void MainActor::giveLeftWeapon(std::shared_ptr<Weapon> w) {
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
void MainActor::giveShieldGenerator(std::shared_ptr<ShieldGenerator> sg) {
	if (m_shield) m_shield->connect_to_energy_source(nullptr);
	m_shield = sg;
	if (m_shield) m_shield->connect_to_energy_source(m_energy_storage);
}
void MainActor::giveTrinket(std::shared_ptr<Trinket> t) {
	m_trinket = t;
}
void MainActor::giveUpgrade(std::shared_ptr<Upgrade> u) {
	m_upgrades.insert(u);
	activate_upgrade(u);
}

std::shared_ptr<EnergyStorage> MainActor::takeEnergyStorage() {
	if (m_engine) m_engine->connect_to_energy_source(nullptr);
	if (m_weapon_left_arm) m_weapon_left_arm->connect_to_energy_source(nullptr);
	if (m_weapon_right_arm) m_weapon_right_arm->connect_to_energy_source(nullptr);
	if (m_shield) m_shield->connect_to_energy_source(nullptr);
	auto ret = m_energy_storage;
	m_energy_storage = nullptr;
	return ret;
}
std::shared_ptr<FlyEngine> MainActor::takeFlyEngine() {
	if (m_engine) m_engine->connect_to_energy_source(nullptr);
	auto ret = m_engine;
	m_engine = nullptr;
	return ret;
}
std::shared_ptr<Weapon> MainActor::takeRightWeapon() {
	m_weapon_right_arm->connect_to_energy_source(nullptr);
	auto ret = m_weapon_right_arm;
	m_weapon_right_arm = nullptr;
	return ret;
}
std::shared_ptr<Weapon> MainActor::takeLeftWeapon() {
	m_weapon_left_arm->connect_to_energy_source(nullptr);
	auto ret = m_weapon_left_arm;
	m_weapon_left_arm = nullptr;
	return ret;
}
std::shared_ptr<ShieldGenerator> MainActor::takeShieldGenerator() {
	m_shield->connect_to_energy_source(nullptr);
	auto ret = m_shield;
	m_shield = nullptr;
	return ret;
}
std::shared_ptr<Trinket> MainActor::takeTrinket() {
	auto ret = m_trinket;
	m_trinket = nullptr;
	return ret;
}
std::shared_ptr<Upgrade> MainActor::takeUpgrade(std::shared_ptr<Upgrade> u) {
	if (u == nullptr) {
		if (!m_upgrades.empty()) {
			auto temp = *m_upgrades.begin();
			deactivate_upgrade(u);
			m_upgrades.erase(u);
			return u;
		}
	} else {
		if (m_upgrades.find(u) != m_upgrades.end()) {
			deactivate_upgrade(u);
			m_upgrades.erase(u);
			return u;
		}
	}
	return nullptr;
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

#include "Objects/EquipableItems/Upgrade.hpp"
void MainActor::activate_upgrade(std::shared_ptr<Upgrade> u) {
	for (auto it : u->get()) {
		if (!(m_energy_storage->upgrade_value(it.first, it.second) ||
			  m_engine->upgrade_value(it.first, it.second) ||
			  m_weapon_left_arm->upgrade_value(it.first, it.second) ||
			  m_weapon_right_arm->upgrade_value(it.first, it.second) ||
			  m_shield->upgrade_value(it.first, it.second) ||
			  m_trinket->upgrade_value(it.first, it.second)))
		{
			throw Exceptions::UnsupportedValueException(("Unsupported value was met at " + u->name()).c_str());
		}
	}
}
void MainActor::deactivate_upgrade(std::shared_ptr<Upgrade> u) {
	for (auto it : u->get()) {
		if (!(m_energy_storage->upgrade_value(it.first, 1.f / it.second) ||
			  m_engine->upgrade_value(it.first, 1.f / it.second) ||
			  m_weapon_left_arm->upgrade_value(it.first, 1.f / it.second) ||
			  m_weapon_right_arm->upgrade_value(it.first, 1.f / it.second) ||
			  m_shield->upgrade_value(it.first, 1.f / it.second) ||
			  m_trinket->upgrade_value(it.first, 1.f / it.second))) 
		{
			throw Exceptions::UnsupportedValueException(("Unsupported value was met at " + u->name()).c_str());
		}
	}
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
		for (auto upgrade : m_upgrades) {
			damage -= upgrade->chance_to_take_damage();
			if (damage <= 0) {
				std::cout << upgrade->name() << ".\n";
				takeUpgrade(upgrade);
				return;
			}
		}
		if (m_trinket) {
			damage -= m_trinket->chance_to_take_damage();
			if (damage <= 0) {
				std::cout << m_trinket->name() << ".\n: ";
				takeTrinket();
				return;
			}
		}
		if (m_shield) {
			damage -= m_shield->chance_to_take_damage();
			if (damage <= 0) {
				std::cout << m_shield->name() << ".\n: ";
				takeShieldGenerator();
				return;
			}
		}
		if (m_weapon_right_arm) {
			damage -= m_weapon_right_arm->chance_to_take_damage();
			if (damage <= 0) {
				std::cout << m_weapon_right_arm->name() << ".\n: ";
				takeRightWeapon();
				return;
			}
		}
		if (m_weapon_left_arm) {
			damage -= m_weapon_left_arm->chance_to_take_damage();
			if (damage <= 0) {
				std::cout << m_weapon_left_arm->name() << ".\n: ";
				takeLeftWeapon();
				return;
			}
		}
		if (m_engine) {
			damage -= m_engine->chance_to_take_damage();
			if (damage <= 0) {
				std::cout << m_engine->name() << ".\n: ";
				takeFlyEngine();
				return;
			}
		}
		if (m_energy_storage) {
			damage -= m_energy_storage->chance_to_take_damage();
			if (damage <= 0) {
				std::cout << m_energy_storage->name() << ".\n: ";
				takeEnergyStorage();
				return;
			}
		}
		std::cout << "his life.\n: ";
		die();
	}
}