#include "PhysicsEngine.hpp"
#include "shared/PhysicsConstants.hpp"
#define time_correction Constants::time_correction_coefficient * float(UpdateInterval)

vector const PhysicsEngine::calculateGravityForce() {
	return vector(0.f, Constants::g);
}
scalar const PhysicsEngine::calculateDimentionalDragForce(scalar const& speed, scalar const& area) {
	if (speed > 0.f)
		return -0.5f * Constants::air_density * speed * speed * Constants::cube_drag_coefficient * area;
	else if (speed < 0.f)
		return +0.5f * Constants::air_density * speed * speed * Constants::cube_drag_coefficient * area;
	else
		return 0.f;
}

vector const PhysicsEngine::calculateDragForce(vector const& speed, vector const& size) {
	return vector(calculateDimentionalDragForce(speed[0], size[1] * size[1]), 
				  calculateDimentionalDragForce(speed[1], size[0] * size[0]));
}

#include "Objects/ObjectState/IndependentObject.hpp"
void PhysicsEngine::processForces(IndependentObjectState *os) {
	auto net_force = calculateGravityForce() +calculateDragForce(os->speed(), os->size());
	auto acceleration = net_force * os->mass();

	os->accelerate(acceleration);
	os->speed_up(os->acceleration(time_correction) * time_correction);
	os->accelerate(-acceleration);
}

#include "Objects/Map/Map.hpp"
#define speed_test(test, result) (test > 0.f ? result : -result)
void PhysicsEngine::processMovement(IndependentObjectState *os, Map *map) {
	if (map) {
		auto future_position = os->position() + os->speed() * time_correction;
		auto half_size = os->size() * 0.5f;
		float speed_multiplier;

		speed_multiplier = map->getSpeedMultiplier(size_t(future_position[0]), size_t(future_position[1] + speed_test(os->speed()[1], half_size[1])));
		os->speed_mul(vector(1.f, speed_multiplier));
		if (speed_multiplier == 0.f) {
			os->stopAcceleration_v();
			if (os->speed()[1] > 0.f)
				os->move_to_v(size_t(future_position[1] + half_size[1]) - half_size[1]);
			else if(os->speed()[1] < 0.f)
				os->move_to_v(size_t(future_position[1] - half_size[1]) + half_size[1] + 1);
		}

		speed_multiplier = map->getSpeedMultiplier(size_t(future_position[0] + speed_test(os->speed()[0], half_size[0])), size_t(future_position[1]));
		os->speed_mul(vector(speed_multiplier, 1.f));
		if (speed_multiplier == 0.f) {
			os->stopAcceleration_h();
			if (os->speed()[0] > 0.f)
				os->move_to_h(size_t(future_position[0] + half_size[0]) - half_size[0]);
			else if(os->speed()[0] < 0.f)
				os->move_to_h(size_t(future_position[0] - half_size[0]) + half_size[0] + 1);
		}
	}
	os->move(vector(os->speed() * time_correction));
}

void PhysicsEngine::processWeaponry(MainActor *ma) {

}