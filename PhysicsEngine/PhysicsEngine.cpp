#include "PhysicsEngine.hpp"

size_t PhysicsEngine::UpdateInterval = 16667ul;

PhysicsEngine::PhysicsEngine() : m_is_initialized(false) {}

PhysicsEngine::PhysicsEngine(std::function<bool()> const& finishFlagAccess) : PhysicsEngine() {
	initialize(finishFlagAccess);
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

#include "PhysicalObjectsQueue.hpp"
void PhysicsEngine::initialize(std::function<bool()> const& finishFlagAccess) {
	m_finish_flag_access = finishFlagAccess;
	m_queue = new PhysicalObjectsQueue();
	m_is_initialized = true;
}
void PhysicsEngine::addObject(AbstractGameObject * object) {
	m_queue->add(object);
}
void PhysicsEngine::removeObject(AbstractGameObject * object) {
	m_queue->remove(object);
}
void PhysicsEngine::clean() {
	if (m_is_initialized && m_queue) 
		delete m_queue;
	m_is_initialized = false;
}

void PhysicsEngine::processQueue() {
	m_queue->for_each([](AbstractGameObject* go) {
		processGravity(go);

		processAcceleration(go);
		processMovement(go);
	});
}
#include <chrono>
#include <thread>
void PhysicsEngine::loop(bool destroy_engine_after_exit) {

	//Initialiation

	while (!m_finish_flag_access()) {
		auto begin_time = std::chrono::steady_clock::now();
		auto next_tick = begin_time + std::chrono::microseconds(UpdateInterval);

		processQueue();

		auto end_time = std::chrono::steady_clock::now();
		//if (end_time > next_tick)
		//	throw Exceptions::ProcessingIsTooSlowException(float((end_time - begin_time).count()) / 1.e+9f);
		std::this_thread::sleep_until(next_tick);
	}
	clean();
	if (destroy_engine_after_exit)
		delete this;
}