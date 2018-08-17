#pragma once
#include <set>
#include <functional>
#include <string>
#include <sstream>
#include <memory>

template <typename Type>
class AbstractQueue {
private:
	std::set<std::shared_ptr<Type>> m_queue;
public:
	AbstractQueue() {}
	~AbstractQueue() {}

	void add(Type* object) { m_queue.insert(std::shared_ptr<Type>(object)); }
	void remove(Type* object) { m_queue.erase(std::shared_ptr<Type>(object)); }
	void for_each(const std::function<void(Type*)> &lambda) { for (auto &it : m_queue) lambda(&*it); }
	void for_each(const std::function<void(Type*)> &lambda) const { for (auto &it : m_queue) lambda(&*it); }
	std::string to_string() const {
		std::ostringstream s;
		s << "Queue ";
		for_each([&s](Type *it) {
			s << std::string(*it);
		});
		return s.str();
	}
	size_t size() const { return m_queue.size(); }
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