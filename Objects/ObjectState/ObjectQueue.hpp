#pragma once
#include <set>
#include <functional>

template <typename Type>
class AbstractQueue {
private:
	std::set<Type*> m_queue;
public:
	AbstractQueue() {}
	~AbstractQueue() {}

	void add(Type* object) { m_queue.insert(object); }
	void remove(Type* object) { m_queue.erase(object); }
	void for_each(const std::function<void(Type*)>& lambda) { for (auto it : m_queue) lambda(it); }
};

#define DefineNewQueue(QueueType, Type) \
	class QueueType : public AbstractQueue<Type> { \
		public: using AbstractQueue::AbstractQueue; \
	};

class IndependentObject;
DefineNewQueue(ObjectQueue, IndependentObject);
class ShootableObject;
DefineNewQueue(ProjectileQueue, ShootableObject);
class Actor;
DefineNewQueue(ActorQueue, Actor);
class MainActor;
DefineNewQueue(MainActorQueue, MainActor);