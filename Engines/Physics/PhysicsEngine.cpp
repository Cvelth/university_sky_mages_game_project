#include "PhysicsEngine.hpp"
#include "Objects/ObjectState/ObjectQueue.hpp"

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
void PhysicsEngine::initializeCollisionSystem(std::shared_ptr<Map> map) {
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
#include "Shared/GameStateController.hpp"
#include "Engines/Networking/NetworkController.hpp"
void PhysicsEngine::loop(bool destroy_engine_after_exit) {
	GameStateController::change_physics_loop_state(true);
	while (!m_finish_flag_access()) {
		auto next_tick = std::chrono::steady_clock::now() + std::chrono::microseconds(UpdateInterval);

		if (GameStateController::state() == GameState::Normal) {
			m_actor_queue->for_each([this](std::shared_ptr<MainActor> go) {
				processForces(go);
				processMovement(go, m_map);
				if (GameStateController::mode() == ProgramMode::Server)
					processWeaponry(go, m_projectile_queue);
			});
			m_projectile_queue->for_each([this](std::shared_ptr<ShootableObject> go) {
				processForces(go);
				processMovement(go, m_map);
			});
			m_object_queue->for_each([this](std::shared_ptr<IndependentObject> go) {
				processForces(go);
				processMovement(go, m_map);
			});

			if (GameStateController::mode() == ProgramMode::Server)
				NetworkController::update_state(m_actor_queue, m_projectile_queue, m_object_queue);
		}
		std::this_thread::sleep_until(next_tick);
	}
	GameStateController::change_physics_loop_state(false);
	clean();
	if (destroy_engine_after_exit)
		delete this;
}