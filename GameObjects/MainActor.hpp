#pragma once
#include "AbstractActors.hpp"
#include "ObjectState.hpp"

class AbstractEnergyStorage;
class AbstractFlyEngine;
class MainActor : public ControllableActor {
protected:
	AbstractEnergyStorage *m_energy_storage;
	AbstractFlyEngine *m_engine;
public:
	using ControllableActor::ControllableActor;
	~MainActor();

	inline void giveEnergyStorage(AbstractEnergyStorage *es) {
		m_energy_storage = es;
	}
	inline void giveFlyEngine(AbstractFlyEngine *fe) {
		m_engine = fe;
	}

public:
	virtual scalar mass() const override;
};