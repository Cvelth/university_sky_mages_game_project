#pragma once
#include <vector>
#include "Objects/Actors/Actor.hpp"
#include "Shared/AbstractException.hpp"
DefineNewException(UnsupportableItemWasGivenException)

class EnergyStorage;
class FlyEngine;
class Weapon;
class ShieldGenerator;
class Trinket;

class ShootableObject;
class Controller;
class PhysicsEngine; 
class MessageInputStream;
class MessageOutputStream;
template <typename Type> class Update;

class MainActor : public Actor {
	friend Controller;
	//friend PhysicsEngine;
protected:
	std::shared_ptr<EnergyStorage> m_energy_storage;
	std::shared_ptr<FlyEngine> m_engine;
	std::shared_ptr<Weapon> m_weapon_left_arm;
	std::shared_ptr<Weapon> m_weapon_right_arm;
	std::shared_ptr<ShieldGenerator> m_shield;
	std::shared_ptr<Trinket> m_trinket;

	float m_aim_x;
	float m_aim_y;

protected:
	std::shared_ptr<ShootableObject> shootRightWeapon(size_t id);
	std::shared_ptr<ShootableObject> shootLeftWeapon(size_t id);
public:
	MainActor(float mass, vector const& acceleration, vector const& speed,
			  vector const& position, vector const& size, std::string const& render_info);
	~MainActor() {}

	void giveEnergyStorage(std::shared_ptr<EnergyStorage> es);
	void giveFlyEngine(std::shared_ptr<FlyEngine> fe);
	void giveRightWeapon(std::shared_ptr<Weapon> w);
	void giveLeftWeapon(std::shared_ptr<Weapon> w);
	void giveShieldGenerator(std::shared_ptr<ShieldGenerator> sg);
	void giveTrinket(std::shared_ptr<Trinket> t);
	std::shared_ptr<EnergyStorage> takeEnergyStorage();
	std::shared_ptr<FlyEngine> takeFlyEngine();
	std::shared_ptr<Weapon> takeRightWeapon();
	std::shared_ptr<Weapon> takeLeftWeapon();
	std::shared_ptr<ShieldGenerator> takeShieldGenerator();
	std::shared_ptr<Trinket> takeTrinket();

	void aim(float x, float y);
	void activateRightWeapon();
	void deactivateRightWeapon();
	void activateLeftWeapon();
	void deactivateLeftWeapon();
	void activateShieldGenerator();
	void deactivateShieldGenerator();

	std::vector<std::shared_ptr<ShootableObject>> shootingProcess(size_t id);
	void was_hit(std::shared_ptr<ShootableObject> so);

	inline std::shared_ptr<EnergyStorage> energy_storage() const { return m_energy_storage; }
	inline std::shared_ptr<FlyEngine> fly_engine() const { return m_engine; }
	inline std::shared_ptr<Weapon> left_weapon() const { return m_weapon_left_arm; }
	inline std::shared_ptr<Weapon> right_weapon() const { return m_weapon_right_arm; }
	inline std::shared_ptr<ShieldGenerator> shield() const { return m_shield; }
	inline std::shared_ptr<Trinket> trinket() const { return m_trinket; }
public:
	virtual scalar mass() const override;
	virtual vector acceleration(scalar const& time_correct) const override;
	virtual vector get_acceleration() const;
	virtual void update_state(vector const& acceleration, vector const& speed, 
							  vector const& position, float capacity_percent);

	friend MessageInputStream& operator>>(MessageInputStream &s, std::shared_ptr<MainActor> &v);
	friend MessageOutputStream& operator<<(MessageOutputStream &s, std::shared_ptr<MainActor> const& v);
	friend MessageInputStream& operator>>(MessageInputStream &s, Update<std::shared_ptr<MainActor>> &v);
	friend MessageOutputStream& operator<<(MessageOutputStream &s, Update<std::shared_ptr<MainActor> const> const& v);
};