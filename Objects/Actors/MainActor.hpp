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
	using Actor::Actor;
	~MainActor();

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
public:
	virtual scalar mass() const override;
	virtual vector acceleration(scalar const& time_correct = 1.f) const override;
};