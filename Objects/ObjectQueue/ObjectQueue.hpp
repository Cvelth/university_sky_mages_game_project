#pragma once
#include <set>
#include <functional>

template <typename Type>
class AbstractQueue {
private:
	std::set<Type*> m_queue;
public:
	ObjectQueue() {}
	~ObjectQueue() {}

	void add(Type* object) { m_queue.insert(object); }
	void remove(Type* object) { m_queue.erase(object); }
	void for_each(const std::function<void(Type*)>& lambda) { for (auto it : m_queue) lambda(it); }
};

class IndependentObject;
using ObjectQueue = AbstractQueue<IndependentObject>;

class Actor;
using ActorQueue = AbstractQueue<Actor>;