#include "PhysicsEngine.hpp"
#include "Objects/ObjectQueue/ObjectQueue.hpp"

size_t PhysicsEngine::UpdateInterval = 16667ul;

PhysicsEngine::PhysicsEngine() : m_is_initialized(false) {}

PhysicsEngine::PhysicsEngine(std::function<bool(void)> const& finishFlagAccess, 
							 MainActorQueue *actor_queue, ProjectileQueue *projectile_queue, ObjectQueue *object_queue) : PhysicsEngine() {
	initialize(finishFlagAccess, actor_queue, projectile_queue, object_queue);
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

void PhysicsEngine::initialize(std::function<bool()> const& finishFlagAccess, 
							   MainActorQueue *actor_queue, ProjectileQueue *projectile_queue, ObjectQueue *object_queue) {
	m_finish_flag_access = finishFlagAccess;

	if (actor_queue != nullptr)
		m_actor_queue = actor_queue;
	else
		m_actor_queue = new MainActorQueue();
	if (projectile_queue != nullptr)
		m_projectile_queue = projectile_queue;
	else
		m_projectile_queue = new ProjectileQueue();
	if (object_queue != nullptr)
		m_object_queue = object_queue;
	else
		m_object_queue = new ObjectQueue();

	m_is_initialized = true;
}
void PhysicsEngine::initializeCollisionSystem(Map *map) {
	m_map = map;
}
void PhysicsEngine::addObject(IndependentObject *object) {
	m_object_queue->add(object);
}
void PhysicsEngine::removeObject(IndependentObject *object) {
	m_object_queue->remove(object);
}
void PhysicsEngine::addActor(MainActor *actor) {
	m_actor_queue->add(actor);
}
void PhysicsEngine::removeActor(MainActor *actor) {
	m_actor_queue->remove(actor);
}
void PhysicsEngine::clean() {
	m_is_initialized = false;
}
#include <iostream>
#include <chrono>
#include <thread>
#include "Objects/AbstractObjects/IndependentObject.hpp"
#include "Objects/Actors/MainActor.hpp"
#include "Objects/AbstractObjects/ShootableObject.hpp"
#include "Shared/GameMode.hpp"
void PhysicsEngine::loop(bool destroy_engine_after_exit) {
	GameModeController::physicsLoopIsReady(true);
	while (!m_finish_flag_access()) {
		auto next_tick = std::chrono::steady_clock::now() + std::chrono::microseconds(UpdateInterval);

		if (GameModeController::getCurrentGameMode() == GameMode::Normal) {
			m_actor_queue->for_each([this](MainActor *go) {
				processForces(go);
				processMovement(go, m_map);
				processWeaponry(go, m_projectile_queue);
			});
			m_projectile_queue->for_each([this](ShootableObject *go) {
				processForces(go);
				processMovement(go, m_map);
			});
			m_object_queue->for_each([this](IndependentObject *go) {
				processForces(go);
				processMovement(go, m_map);
			});
		}

		std::this_thread::sleep_until(next_tick);
	}
	GameModeController::physicsLoopIsReady(false);
	clean();
	if (destroy_engine_after_exit)
		delete this;
}