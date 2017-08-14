#include "AbstractObjectQueue.hpp"
#include "AbstractGameObject.hpp"

AbstractObjectQueue::AbstractObjectQueue() {}

AbstractObjectQueue::~AbstractObjectQueue() {}

void AbstractObjectQueue::add(AbstractGameObject* object) {
	m_queue.insert(object);
	object->wasAddedToQueue();
}

void AbstractObjectQueue::remove(AbstractGameObject* object) {
	m_queue.erase(object);
	object->wasRemovedFromQueue();
}

void AbstractObjectQueue::for_each(const std::function<void(AbstractGameObject*)>& lambda) {
	for (auto it : m_queue)
		lambda(it);
}
