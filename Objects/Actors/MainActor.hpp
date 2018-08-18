#pragma once
#include <vector>
#include "Objects/AbstractObjects/ActorObject.hpp"
#include "Shared/AbstractException.hpp"
DefineNewException(UnsupportableItemWasGivenException)

class EnergyStorage;
class FlyEngine;
class Weapon;
class ShootableObject;
class Controller;
class PhysicsEngine;
class MainActor : public Actor {
	friend Controller;
	//friend PhysicsEngine;
protected:
	EnergyStorage *m_energy_storage;
	FlyEngine *m_engine;
	Weapon *m_weapon_left_arm;
	Weapon *m_weapon_right_arm;

	float m_aim_x;
	float m_aim_y;

protected:
	ShootableObject* shootRightWeapon();
	ShootableObject* shootLeftWeapon();
public:
	MainActor(float mass, vector const& acceleration, vector const& speed,
			  vector const& position, vector const& size, std::shared_ptr<RenderInfo> render_info);
	~MainActor();
	operator std::string() const;

	void giveEnergyStorage(EnergyStorage *es);
	void giveFlyEngine(FlyEngine *fe);
	void giveRightWeapon(Weapon *w);
	void giveLeftWeapon(Weapon *w);

	void activateRightWeapon();
	void activateLeftWeapon();
	void aim(float x, float y);
	void deactivateRightWeapon();
	void deactivateLeftWeapon();

	std::vector<ShootableObject*> shootingProcess();

	EnergyStorage* energy_storage() const { return m_energy_storage; }
	FlyEngine* fly_engine() const { return m_engine; }
	Weapon* left_weapon() const { return m_weapon_left_arm; }
	Weapon* right_weapon() const { return m_weapon_right_arm; }
public:
	virtual scalar mass() const override;
	virtual vector acceleration(scalar const& time_correct) const override;
	virtual vector get_acceleration() const;
	virtual void update_state(vector const& acceleration, vector const& speed, vector const& position);
};