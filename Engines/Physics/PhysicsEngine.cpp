#include "PhysicsEngine.hpp"
#include "Objects/ObjectQueue/ObjectQueue.hpp"

size_t PhysicsEngine::UpdateInterval = 16667ul;

PhysicsEngine::PhysicsEngine() : m_is_initialized(false) {}

PhysicsEngine::PhysicsEngine(std::function<bool(void)> const& finishFlagAccess, 
							 ObjectQueue* queue) : PhysicsEngine() {
	initialize(finishFlagAccess, queue);
}

PhysicsEngine::~PhysicsEngine() {
	clean();
}

size_t PhysicsEngine::getUpdateInterval() {
	return UpdateInterval;
}

void PhysicsEngine::changeUpdateInterval(size_t microseconds) {
	UpdateInterval = microseconds;
}

void PhysicsEngine::initialize(std::function<bool()> const& finishFlagAccess, ObjectQueue* queue) {
	m_finish_flag_access = finishFlagAccess;

	if (queue != nullptr)
		m_queue = queue;
	else
		m_queue = new ObjectQueue();

	m_is_initialized = true;
}
void PhysicsEngine::initializeCollisionSystem(Map *map) {
	m_map = map;
}
void PhysicsEngine::addObject(IndependentObject *object) {
	m_queue->add(object);
}
void PhysicsEngine::removeObject(IndependentObject *object) {
	m_queue->remove(object);
}
void PhysicsEngine::clean() {
	m_is_initialized = false;
}
#include <iostream>
#include <chrono>
#include <thread>
#include "Objects/AbstractObjects/IndependentObject.hpp"
#include "Shared/GameMode.hpp"
void PhysicsEngine::loop(bool destroy_engine_after_exit) {
	GameModeController::physicsLoopIsReady(true);
	while (!m_finish_flag_access()) {
		auto next_tick = std::chrono::steady_clock::now() + std::chrono::microseconds(UpdateInterval);

		if (GameModeController::getCurrentGameMode() == GameMode::Normal)
			m_queue->for_each([this](IndependentObject* go) {
				processForces(go);
				processMovement(go, m_map);
			});

		std::this_thread::sleep_until(next_tick);
	}
	GameModeController::physicsLoopIsReady(false);
	clean();
	if (destroy_engine_after_exit)
		delete this;
}