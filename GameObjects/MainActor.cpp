#include "MainActor.hpp"
#include "AbstractEquipableItems.hpp"

MainActor::~MainActor() {
	delete m_energy_storage;
delete m_engine;
}
