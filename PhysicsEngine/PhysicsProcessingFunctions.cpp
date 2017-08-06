#include "PhysicsEngine.hpp"
#include "PhysicalObjectsQueue.hpp"
#include "PhysicalConstants.hpp"
#include "GameObjects\ObjectState.hpp"

#define time_correction 1e-6f * float(UpdateInterval)
#define speed_test(test, result) (test > 0.f ? result : -result)

void PhysicsEngine::processGravity(IndependentObjectState *os) {
	os->accelerate(vector(0.f, os->mass() * Constants::g * time_correction));
}

#include "LogicEngine\GameMap.hpp"
void PhysicsEngine::processMovement(IndependentObjectState *os, GameMap *map) {
	os->speed_up(os->acceleration() * time_correction);

	if (map) {
		auto future_position = os->position() + os->speed() * time_correction;
		auto half_size = os->size() * 0.5f;
		float speed_multiplier;

		speed_multiplier = map->getSpeedMultiplier(size_t(future_position.h), size_t(future_position.v + speed_test(os->speed().v, half_size.v)));
		os->speed_mul(vector(1.f, speed_multiplier));
		if (speed_multiplier == 0.f) {
			os->stopAcceleration_v();
			os->move_to_v(size_t(future_position.v + speed_test(os->speed().v, half_size.v)) - speed_test(os->speed().v, half_size.v));
		}

		speed_multiplier = map->getSpeedMultiplier(size_t(future_position.h + speed_test(os->speed().h, half_size.h)), size_t(future_position.v));
		os->speed_mul(vector(speed_multiplier, 1.f));
		if (speed_multiplier == 0.f) {
			os->stopAcceleration_h();
			os->move_to_h(size_t(future_position.h + speed_test(os->speed().h, half_size.h)) - speed_test(os->speed().h, half_size.h));
		}
	}
	os->move(vector(os->speed() * time_correction));
}