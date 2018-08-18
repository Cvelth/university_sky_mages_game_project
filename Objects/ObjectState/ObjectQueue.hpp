#pragma once
#include <memory>
#include <functional>
#include <string>
#include <sstream>
template <typename Type>
class AbstractQueueInterface {
public:
	virtual void add(Type* object) abstract;
	virtual void remove(Type* object) abstract;
	virtual void for_each(const std::function<void(std::shared_ptr<Type>)> &lambda) abstract;
	virtual void for_each(const std::function<void(std::shared_ptr<Type>)> &lambda) const abstract;
	virtual size_t size() const abstract;
	virtual void clear() abstract;

	virtual std::string to_string() const {
		std::ostringstream s;
		s << "Queue\n";
		for_each([&s](std::shared_ptr<Type> it) {
			s << std::string(*it);
		});
		return s.str();
	}
};
#include <set>
template <typename Type>
class AbstractSet : public AbstractQueueInterface<Type> {
private:
	std::set<std::shared_ptr<Type>> m_queue;
public:
	virtual void add(Type* object) override { m_queue.insert(std::shared_ptr<Type>(object)); }
	virtual void remove(Type* object) override { m_queue.erase(std::shared_ptr<Type>(object)); }
	virtual void for_each(const std::function<void(std::shared_ptr<Type>)> &lambda) override { for (auto &it : m_queue) lambda(it); }
	virtual void for_each(const std::function<void(std::shared_ptr<Type>)> &lambda) const override { for (auto &it : m_queue) lambda(it); }
	virtual size_t size() const override { return m_queue.size(); }
	virtual void clear() override { m_queue.clear(); }
};
#include <list>
template <typename Type>
class AbstractList : public AbstractQueueInterface<Type> {
private:
	std::list<std::shared_ptr<Type>> m_queue;
public:
	virtual void add(Type* object) override { m_queue.push_back(std::shared_ptr<Type>(object)); }
	virtual void remove(Type* object) override { m_queue.erase(std::find(m_queue.begin(), m_queue.end(), std::shared_ptr<Type>(object))); }
	virtual void for_each(const std::function<void(std::shared_ptr<Type>)> &lambda) override { for (auto &it : m_queue) lambda(it); }
	virtual void for_each(const std::function<void(std::shared_ptr<Type>)> &lambda) const override { for (auto &it : m_queue) lambda(it); }
	virtual size_t size() const override { return m_queue.size(); }
	virtual void clear() override { m_queue.clear(); }
};
#include <vector>
template <typename Type>
class AbstractVector : public AbstractQueueInterface<Type> {
private:
	std::vector<std::shared_ptr<Type>> m_queue;
public:
	virtual void add(Type* object) override { m_queue.push_back(std::shared_ptr<Type>(object)); }
	virtual void remove(Type* object) override { m_queue.erase(std::find(m_queue.begin(), m_queue.end(), std::shared_ptr<Type>(object))); }
	virtual void for_each(const std::function<void(std::shared_ptr<Type>)> &lambda) override { for (auto &it : m_queue) lambda(it); }
	virtual void for_each(const std::function<void(std::shared_ptr<Type>)> &lambda) const override { for (auto &it : m_queue) lambda(it); }
	virtual size_t size() const override { return m_queue.size(); }
	virtual void clear() override { m_queue.clear(); }

	virtual std::shared_ptr<Type> at(size_t index) { return m_queue.at(index); }
	virtual std::shared_ptr<Type> const at(size_t index) const { return m_queue.at(index); }
	virtual std::shared_ptr<Type> const operator[](size_t index) const { return m_queue.at(index); }
	virtual std::shared_ptr<Type> operator[](size_t index) { return m_queue.at(index); }
};

#define DefineNewQueue(QueueType, Type) \
	class QueueType : public AbstractQueue<Type> { \
		public: using AbstractQueue::AbstractQueue; \
	};

class IndependentObject;
class ShootableObject;
class MainActor;
class ObjectQueue : public AbstractSet<IndependentObject> { public: using AbstractSet::AbstractSet; };
class ProjectileQueue : public AbstractSet<ShootableObject> { public: using AbstractSet::AbstractSet; };
class MainActorQueue : public AbstractVector<MainActor> { public: using AbstractVector::AbstractVector; };

#include "Objects/Actors/MainActor.hpp"
#include "Objects/AbstractObjects/ShootableObject.hpp"