#pragma once
#include "AbstractActors.hpp"
#include "ObjectState.hpp"

class AbstractEnergyStorage;
class AbstractFlyEngine;
class AbstractWeapon;
class GameController;
class MainActor : public ControllableActor {
	friend GameController;
protected:
	AbstractEnergyStorage *m_energy_storage;
	AbstractFlyEngine *m_engine;
	AbstractWeapon *m_weapon_left_arm;
	AbstractWeapon *m_weapon_right_arm;

	float m_aim_x;
	float m_aim_y;
public:
	using ControllableActor::ControllableActor;
	~MainActor();

	inline void giveEnergyStorage(AbstractEnergyStorage *es) {
		m_energy_storage = es;
	}
	inline void giveFlyEngine(AbstractFlyEngine *fe) {
		m_engine = fe;
	}
	void giveLeftWeapon(AbstractWeapon *w);
	void giveRightWeapon(AbstractWeapon *w);

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