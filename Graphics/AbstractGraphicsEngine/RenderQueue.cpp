#include "RenderQueue.hpp"

RenderQueue::RenderQueue() {}

RenderQueue::~RenderQueue() {}

void RenderQueue::add(AbstractGameObject* object) {
	m_queue.insert(object);
}

void RenderQueue::remove(AbstractGameObject * object) {
	m_queue.erase(object);
}