#include "EnergyStorage.hpp"
#include "Shared/TimeControl.hpp"
EnergyStorage::EnergyStorage(float capacity) : EquipableItem(), m_maximum_capacity(capacity), m_current_capacity(capacity) {
	m_energy_loss_time = now();
}

#include <cmath>
void EnergyStorage::calculate_energy_loss() {
	auto time = now();
	m_current_capacity /= exp(m_energy_percent_loss_per_second * float(time - m_energy_loss_time) / 1.e6f);
	if (m_current_capacity < 1.f)
		m_current_capacity = 0;
	m_energy_loss_time = time;
}