#include "PhysicsEngine.hpp"
#include "PhysicalConstants.hpp"
#define time_correction Constants::time_correction_coefficient * float(UpdateInterval)

vector const PhysicsEngine::calculateGravityForce() {
	return vector(0.f, Constants::g);
}
scalar const PhysicsEngine::calculateDimentionalDragForce(scalar const& speed, scalar const& area) {
	if (speed > 0.f)
		return -0.5f * Constants::air_density * speed * speed * Constants::cube_drag_coefficient * area;
	else if(speed < 0.f)
		return +0.5f * Constants::air_density * speed * speed * Constants::cube_drag_coefficient * area;
}

vector const PhysicsEngine::calculateDragForce(vector const& speed, vector const& size) {
	return vector(calculateDimentionalDragForce(speed.h, size.v * size.v), 
				  calculateDimentionalDragForce(speed.v, size.h * size.h));
}

#include "GameObjects\ObjectState.hpp"
void PhysicsEngine::processForces(IndependentObjectState *os) {
	auto net_force = calculateGravityForce() +calculateDragForce(os->speed(), os->size());
	auto acceleration = net_force * os->mass();

	os->accelerate(acceleration);
	os->speed_up(os->acceleration() * time_correction);
	os->accelerate(-acceleration);
}

#include "LogicEngine\GameMap.hpp"
#define speed_test(test, result) (test > 0.f ? result : -result)
void PhysicsEngine::processMovement(IndependentObjectState *os, GameMap *map) {
	if (map) {
		auto future_position = os->position() + os->speed() * time_correction;
		auto half_size = os->size() * 0.5f;
		float speed_multiplier;

		speed_multiplier = map->getSpeedMultiplier(size_t(future_position.h), size_t(future_position.v + speed_test(os->speed().v, half_size.v)));
		os->speed_mul(vector(1.f, speed_multiplier));
		if (speed_multiplier == 0.f) {
			os->stopAcceleration_v();
			if (os->speed().v > 0.f)
				os->move_to_v(size_t(future_position.v + half_size.v) - half_size.v);
			else if(os->speed().v < 0.f)
				os->move_to_v(size_t(future_position.v - half_size.v) + half_size.v + 1);
		}

		speed_multiplier = map->getSpeedMultiplier(size_t(future_position.h + speed_test(os->speed().h, half_size.h)), size_t(future_position.v));
		os->speed_mul(vector(speed_multiplier, 1.f));
		if (speed_multiplier == 0.f) {
			os->stopAcceleration_h();
			if (os->speed().h > 0.f)
				os->move_to_h(size_t(future_position.h + half_size.h) - half_size.h);
			else if(os->speed().h < 0.f)
				os->move_to_h(size_t(future_position.h - half_size.h) + half_size.h + 1);
		}
	}
	os->move(vector(os->speed() * time_correction));
}