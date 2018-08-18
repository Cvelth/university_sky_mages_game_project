#include "FlyEngine.hpp"
#include "EnergyStorage.hpp"
#include "Shared/PhysicsConstants.hpp"

float const FlyEngine::calculate_acceleration_up() const {
	return -m_maximum_acceleration * m_up_acceleration_percent + m_anti_gravity_expected_mass * Constants::g;
}
float const FlyEngine::calculate_acceleration_down() const {
	return m_maximum_acceleration * m_down_acceleration_percent + m_anti_gravity_expected_mass * Constants::g;
}
float const FlyEngine::calculate_acceleration_left() const {
	return -m_maximum_acceleration * m_left_acceleration_percent - m_anti_gravity_expected_mass * Constants::g;
}
float const FlyEngine::calculate_acceleration_right() const {
	return m_maximum_acceleration * m_right_acceleration_percent + m_anti_gravity_expected_mass * Constants::g;
}

vector FlyEngine::acceleration(scalar const & time_correct) const {
	static_assert(true, "The function must not be used.");
	auto ret = DependedAcceleratableObjectState::acceleration();
	ret[1] -= m_anti_gravity_expected_mass * Constants::g;
	return ret;
}
vector FlyEngine::acceleration(scalar const& mass, scalar const& time_correct) const {
	auto ret = DependedAcceleratableObjectState::acceleration();
	ret[1] -= m_anti_gravity_expected_mass * Constants::g;
	if (ret[0] == 0 && ret[1] == 0)
		return vector(0.f, 0.f);
	if (m_energy_source && !m_energy_source->isEmpty() && m_energy_source->use(ret.length() * mass * time_correct / m_energy_usage_coefficient))
		return ret;
	else
		return vector(0.f, 0.f);
}
vector FlyEngine::get_acceleration() const {
	return DependedAcceleratableObjectState::m_acceleration;
}
void FlyEngine::update_acceleration(vector const& acceleration) {
	DependedAcceleratableObjectState::m_acceleration = acceleration;
}
