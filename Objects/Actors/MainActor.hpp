#pragma once
#include "Objects\AbstractObjects\ActorObject.hpp"
#include "Shared\AbstractException.hpp"
DefineNewException(UnsupportableItemWasGivenException)

class EnergyStorage;
class FlyEngine;
class Weapon;
class GameController;
class MainActor : public Actor {
	friend GameController;
protected:
	EnergyStorage *m_energy_storage;
	FlyEngine *m_engine;
	Weapon *m_weapon_left_arm;
	Weapon *m_weapon_right_arm;

	float m_aim_x;
	float m_aim_y;
public:
	using Actor::Actor;
	~MainActor();

	inline void giveEnergyStorage(EnergyStorage *es) {
		m_energy_storage = es;
	}
	inline void giveFlyEngine(FlyEngine *fe) {
		m_engine = fe;
	}
	void giveLeftWeapon(Weapon *w);
	void giveRightWeapon(Weapon *w);

	void activateRightWeapon();
	void activateLeftWeapon();
	void aim(float x, float y);
	void deactivateRightWeapon();
	void deactivateLeftWeapon();

	void execute_shooting();

public:
	virtual scalar mass() const override;
	virtual vector acceleration(scalar const& time_correct = 1.f) const override;
};