#include "MainActor.hpp"
#include "AbstractEquipableItems.hpp"

MainActor::~MainActor() {
	if (m_energy_storage) delete m_energy_storage;
	if (m_engine) delete m_engine;
}

scalar MainActor::mass() const {
	return m_mass + 
		(m_engine ? m_engine->mass() : 0.f) + 
		(m_energy_storage ? m_energy_storage->mass() : 0.f);
}

vector MainActor::acceleration(scalar const& time_correct) const {
	return m_acceleration +
		(m_engine ? m_engine->acceleration(mass(), time_correct) : vector(0.f, 0.f));
}