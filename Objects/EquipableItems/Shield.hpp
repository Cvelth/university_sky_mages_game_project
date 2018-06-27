#pragma once
#include "Objects/AbstractObjects/EquipableItem.hpp"

enum class ShieldType {
	Anti_Energy, Anti_Projectile, Anti_Bullet
};
class ShieldGenerator : public EquipableItem {
private:
	ShieldType m_type;
protected:

public:
	ShieldGenerator(ShieldType type, float mass) : EquipableItem(mass), m_type(type) {}
	~ShieldGenerator() {}
};
