#pragma once
#include <set>
class AbstractGameObject;

class PhysicalObjectsQueue {
private:
	std::set<AbstractGameObject*> m_queue;
public:
	PhysicalObjectsQueue();
	~PhysicalObjectsQueue();

	void add(AbstractGameObject* object);
	void remove(AbstractGameObject* object);
};
