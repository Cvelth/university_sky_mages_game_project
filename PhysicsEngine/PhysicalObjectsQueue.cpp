#include "PhysicalObjectsQueue.hpp"

PhysicalObjectsQueue::PhysicalObjectsQueue() {}
PhysicalObjectsQueue::~PhysicalObjectsQueue() {}

void PhysicalObjectsQueue::add(AbstractGameObject* object) {
	m_queue.insert(object);
}

void PhysicalObjectsQueue::remove(AbstractGameObject* object) {
	m_queue.erase(object);
}
