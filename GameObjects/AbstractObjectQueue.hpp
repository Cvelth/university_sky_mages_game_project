#pragma once
#include <set>
#include <functional>
class AbstractGameObject;

class AbstractObjectQueue {
private:
	std::set<AbstractGameObject*> m_queue;
public:
	AbstractObjectQueue();
	~AbstractObjectQueue();

	void add(AbstractGameObject* object);
	void remove(AbstractGameObject* object);

	void for_each(const std::function<void(AbstractGameObject*)>& lambda);
};
