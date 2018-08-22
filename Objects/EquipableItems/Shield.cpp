#include "Shield.hpp"
#include "../AbstractObjects/ShootableObject.hpp"
void ShieldGenerator::was_hit(ShootableObject *so) {
	//To implement.
}

template<>
inline void ShieldGenerator::set_value<std::string>(std::string const& name, std::string const& value) {
	if (name == "name")
		m_name = value;
	else if (name == "description")
		m_description = value;
	else
		throw Exceptions::UnsupportedValueException("Unsupported value was passed");
}
template<>
inline void ShieldGenerator::set_value<float>(std::string const& name, float const& value) {
	if (name == "mass")
		addMass(value);
	else if (name == "anti_energy_efficiency")
		m_anti_energy_efficiency = value;
	else if (name == "anti_projectile_efficiency")
		m_anti_projectile_efficiency = value;
	else if (name == "anti_bullet_efficiency")
		m_anti_bullet_efficiency = value;
	else if (name == "activation_energy")
		m_activation_energy = value;
	else if (name == "energy_per_second")
		m_energy_per_second = value;
	else
		throw Exceptions::UnsupportedValueException("Unsupported value was passed");
}