#include "PhysicsEngine.hpp"
#include "PhysicalObjectsQueue.hpp"
#include "PhysicalConstants.hpp"
#include "GameObjects\AbstractGameObject.hpp"

#define time_correction 1e-6f * float(UpdateInterval)
#define speed_test(test, result) (test > 0.f ? result : -result)

void PhysicsEngine::processGravity(ObjectState &os) {
	os.acceleration.v = os.mass * Constants::g * time_correction;
}

#include "GameLogicEngine\GameMap.hpp"
void PhysicsEngine::processMovement(ObjectState &os, GameMap *map) {
	os.speed += os.acceleration * time_correction;

	if (map) {
		auto future_position = os.position + os.speed * time_correction;
		auto half_size = os.size * 0.5f;
		float speed_multiplier;

		speed_multiplier = map->getSpeedMultiplier(size_t(future_position.h), size_t(future_position.v + speed_test(os.speed.v, half_size.v)));
		os.speed.v *= speed_multiplier;
		if (speed_multiplier == 0.f) {
			os.acceleration.v = 0.f;
			os.position.v = size_t(future_position.v + speed_test(os.speed.v, half_size.v)) - speed_test(os.speed.v, half_size.v);
		}

		speed_multiplier = map->getSpeedMultiplier(size_t(future_position.h + speed_test(os.speed.h, half_size.h)), size_t(future_position.v));
		os.speed.h *= speed_multiplier;
		if (speed_multiplier == 0.f) {
			os.acceleration.h = 0.f;
			os.position.h = size_t(future_position.h + speed_test(os.speed.h, half_size.h)) - speed_test(os.speed.h, half_size.h);
		}
	} 
	os.position += os.speed * time_correction;
}