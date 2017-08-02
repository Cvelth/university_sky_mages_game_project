#include "PhysicsEngine.hpp"
#include "PhysicalObjectsQueue.hpp"
#include "PhysicalConstants.hpp"
#include "GameObjects\AbstractGameObject.hpp"

void PhysicsEngine::processGravity(AbstractGameObject* go) {
	float time_correction = 1e-6f * UpdateInterval;
	go->accelerate(0.f, go->mass() * Constants::g * time_correction);
}

void PhysicsEngine::processAcceleration(AbstractGameObject * go) {
	go->updateSpeed();
}

void PhysicsEngine::processMovement(AbstractGameObject * go) {
	go->updatePosition();
}
