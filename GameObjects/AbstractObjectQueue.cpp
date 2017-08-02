#include "AbstractObjectQueue.hpp"

AbstractObjectQueue::AbstractObjectQueue() {}

AbstractObjectQueue::~AbstractObjectQueue() {}

void AbstractObjectQueue::add(AbstractGameObject* object) {
	m_queue.insert(object);
}

void AbstractObjectQueue::remove(AbstractGameObject* object) {
	m_queue.erase(object);
}
