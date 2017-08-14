#pragma once
#include "Objects\AbstractObjects\EquipableItem.hpp"

class AbstractTrinket : public EquipableItem {
public:
	AbstractTrinket(float mass) : EquipableItem(mass) {}
	~AbstractTrinket() {}
};