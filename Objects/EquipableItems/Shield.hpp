#pragma once
#include "Objects/AbstractObjects/EquipableItem.hpp"

class ShootableObject;
class EnergyStorage;
class ObjectStorage;
class ShieldGenerator : public EquipableItem {
	friend ObjectStorage;
private:
	float m_anti_energy_efficiency;
	float m_anti_projectile_efficiency;
	float m_anti_bullet_efficiency;

	float m_activation_energy;
	float m_energy_per_second;
	EnergyStorage *m_energy_source;

	mutable bool is_activated;
protected:
	ShieldGenerator() : EquipableItem() {}
public:
	~ShieldGenerator() {}
	void connect_to_energy_source(EnergyStorage *source) { m_energy_source = source; }

	inline void activate() const {
		is_activated = true;
	}
	inline void deactivate() const {
		is_activated = false;
	}
	void was_hit(ShootableObject *so);
private:
	template <typename value_type>
	void set_value(std::string const& name, value_type const& value);
};

template<typename value_type>
inline void ShieldGenerator::set_value(std::string const& name, value_type const& value) {
	static_assert("Unsupported value type");
}
