#include "ObjectQueue.hpp"

ObjectQueue::ObjectQueue() {}
ObjectQueue::~ObjectQueue() {}
void ObjectQueue::add(IndependentObject* object) {
	m_queue.insert(object);
}
void ObjectQueue::remove(IndependentObject* object) {
	m_queue.erase(object);
}
void ObjectQueue::for_each(const std::function<void(IndependentObject*)>& lambda) {
	for (auto it : m_queue)
		lambda(it);
}