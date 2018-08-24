#include "PhysicsEngine.hpp"
#include "Objects/Objects/ObjectQueue.hpp"

size_t PhysicsEngine::UpdateInterval = 16667ul;

PhysicsEngine::PhysicsEngine(std::function<bool(void)> const& finishFlagAccess,
							 MainActorQueue &actor_queue, DoubleProjectileQueue &projectile_queue, ObjectQueue &object_queue)
	: m_actor_queue(actor_queue), m_projectile_queue(projectile_queue), m_object_queue(object_queue), m_finish_flag_access(finishFlagAccess) {}

size_t PhysicsEngine::getUpdateInterval() {
	return UpdateInterval;
}

void PhysicsEngine::changeUpdateInterval(size_t microseconds) {
	UpdateInterval = microseconds;
}
void PhysicsEngine::initializeCollisionSystem(std::shared_ptr<Map> map) {
	m_map = map;
}
#include <iostream>
#include <chrono>
#include <thread>
#include "Objects/Objects/IndependentObject.hpp"
#include "Objects/Actors/MainActor.hpp"
#include "Objects/Objects/ShootableObject.hpp"
#include "Shared/GameStateController.hpp"
#include "Engines/Networking/Networking.hpp"
#include "Engines/Networking/Message.hpp"
void PhysicsEngine::loop(bool destroy_engine_after_exit) {
	GameStateController::change_physics_loop_state(true);
	while (!m_finish_flag_access()) {
		auto next_tick = std::chrono::steady_clock::now() + std::chrono::microseconds(UpdateInterval);

		if (GameStateController::state() == GameState::Normal) {
			m_actor_queue.for_each([this](std::shared_ptr<MainActor> go, size_t id) {
				if (go->is_alive()) {
					processForces(go);
					processMovement(go, m_map);
					if (GameStateController::mode() == ProgramMode::Server)
						processWeaponry(go, m_projectile_queue, id);
				}
			});
			if (GameStateController::mode() == ProgramMode::Server)
				m_projectile_queue.clone();
			m_projectile_queue->for_each([this](std::shared_ptr<ShootableObject> go) {
				processForces(go);
				if (processMovement(go, m_map) && GameStateController::mode() == ProgramMode::Server)
					m_projectile_queue.next().remove(go);
				if (GameStateController::mode() == ProgramMode::Server) 
					if (processTargeting(go, m_actor_queue))
						m_projectile_queue.next().remove(go);
			});
			if (GameStateController::mode() == ProgramMode::Server)
				m_projectile_queue.swap();
			m_object_queue.for_each([this](std::shared_ptr<IndependentObject> go) {
				processForces(go);
				processMovement(go, m_map);
			});
			
			if (GameStateController::mode() == ProgramMode::Server) {
				Networking::bcast_from_server(make_actor_queue_update_message(m_actor_queue));
				Networking::bcast_from_server(make_projectile_queue_message(*m_projectile_queue));
			}
		}
		std::this_thread::sleep_until(next_tick);
	}
	GameStateController::change_physics_loop_state(false);
	if (destroy_engine_after_exit)
		delete this;
}