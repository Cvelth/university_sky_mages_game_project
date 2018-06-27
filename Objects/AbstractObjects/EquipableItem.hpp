#pragma once
#include "Objects/ObjectState/DependentObject.hpp"
class EquipableItem : public DependentObjectState {
public:
	EquipableItem(float mass)
		: DependentObjectState(mass) {}
	~EquipableItem() {}
};