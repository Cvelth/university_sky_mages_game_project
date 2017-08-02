#pragma once
#include <set>
class AbstractGameObject;

class AbstractObjectQueue {
private:
	std::set<AbstractGameObject*> m_queue;
public:
	AbstractObjectQueue();
	~AbstractObjectQueue();

	void add(AbstractGameObject* object);
	void remove(AbstractGameObject* object);

	inline std::set<AbstractGameObject*>& get() {
		return m_queue;
	}
};
