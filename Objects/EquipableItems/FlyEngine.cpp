#include "FlyEngine.hpp"
#include "EnergyStorage.hpp"
#include "Shared\PhysicsConstants.hpp"

vector FlyEngine::acceleration(scalar const & time_correct) const {
	throw Exceptions::YouMustNotUseThisFunctionExceptions("The function must not be used.");
	auto ret = DependedAcceleratableObjectState::acceleration();
	ret[1] -= m_anti_gravity_expected_mass * Constants::g;
	return ret;
}

vector FlyEngine::acceleration(scalar const & mass, scalar const & time_correct) const {
	auto ret = DependedAcceleratableObjectState::acceleration();
	ret[1] -= m_anti_gravity_expected_mass * Constants::g;
	if (ret[0] == 0 && ret[1] == 0)
		return vector(0.f, 0.f);
	if (!m_energy_source->isEmpty() && m_energy_source->use(ret.length() * mass * time_correct * m_energy_usage_coefficient))
		return ret;
	else
		return vector(0.f, 0.f);
}