#pragma once
#include "GameObjects\AbstractGameObject.hpp"
#include <set>

class RenderQueue {
private:
	std::set<AbstractGameObject*> m_queue;
public:
	RenderQueue();
	~RenderQueue();

	void add(AbstractGameObject* object);
	void remove(AbstractGameObject* object);
};
