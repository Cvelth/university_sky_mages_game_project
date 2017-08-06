#include "MainActor.hpp"
#include "AbstractEquipableItems.hpp"

MainActor::~MainActor() {
	delete m_energy_storage;
	delete m_engine;
}

scalar MainActor::mass() const {
	return m_mass + m_engine->mass() + m_energy_storage->mass();
}