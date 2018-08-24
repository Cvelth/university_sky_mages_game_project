#pragma once
#include <memory>
#include <functional>
#include <string>
#include <sstream>
template <typename Type>
class AbstractQueueInterface {
public:
	virtual void add(std::shared_ptr<Type> object) abstract;
	virtual void remove(std::shared_ptr<Type> object) abstract;
	virtual void for_each(const std::function<void(std::shared_ptr<Type>)> &lambda) abstract;
	virtual void for_each(const std::function<void(std::shared_ptr<Type>)> &lambda) const abstract;
	virtual size_t size() const abstract;
	virtual void clear() abstract;
};
#include <array>
template <typename QueueType, size_t queue_number> 
class MultiQueue {
private:
	size_t m_current_queue;
	std::array<QueueType, queue_number> m_queues;
protected:
	inline typename std::enable_if<queue_number >= 2, size_t>::type next_queue() const {
		if (m_current_queue == queue_number - 1) return 0; else return m_current_queue + 1; 
	}
public:
	MultiQueue() : m_current_queue(0u) {}
	inline QueueType& operator*() { return m_queues[m_current_queue]; }
	inline QueueType const& operator*() const { return m_queues[m_current_queue]; }
	inline QueueType* operator->() { return &m_queues[m_current_queue]; }
	inline QueueType const* operator->() const { return &m_queues[m_current_queue]; }
	inline QueueType& get() { return m_queues[m_current_queue]; }
	inline QueueType const& get() const { return m_queues[m_current_queue]; }

	inline typename std::enable_if<queue_number >= 2, QueueType&>::type next() {
		return m_queues[next_queue()];
	}
	inline typename std::enable_if<queue_number >= 2, QueueType const&>::type next() const {
		return m_queues[next_queue()];
	}
	inline typename std::enable_if<queue_number >= 2>::type swap() {
		m_current_queue = next_queue();
	}

	inline typename std::enable_if<queue_number >= 2>::type clone() {
		auto next = next_queue();
		m_queues[next].clear();
		m_queues[next] = get();
	}
};
#include <set>
template <typename Type>
class AbstractSet : public AbstractQueueInterface<Type> {
private:
	std::set<std::shared_ptr<Type>> m_queue;
public:
	virtual void add(std::shared_ptr<Type> object) override { m_queue.insert(object); }
	virtual void remove(std::shared_ptr<Type> object) override { m_queue.erase(object); }
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
	virtual void add(std::shared_ptr<Type> object) override { m_queue.push_back(object); }
	virtual void remove(std::shared_ptr<Type> object) override { m_queue.erase(std::find(m_queue.begin(), m_queue.end(), object)); }
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
	virtual void add(std::shared_ptr<Type> object) override { m_queue.push_back(object); }
	virtual void remove(std::shared_ptr<Type> object) override { m_queue.erase(std::find(m_queue.begin(), m_queue.end(), object)); }
	virtual void for_each(const std::function<void(std::shared_ptr<Type>)> &lambda) override { for (auto &it : m_queue) lambda(it); }
	virtual void for_each(const std::function<void(std::shared_ptr<Type>)> &lambda) const override { for (auto &it : m_queue) lambda(it); }
	virtual void for_each(const std::function<void(std::shared_ptr<Type>, size_t)> &lambda) { size_t i = 0u; for (auto &it : m_queue) lambda(it, i++); }
	virtual void for_each(const std::function<void(std::shared_ptr<Type>, size_t)> &lambda) const { size_t i = 0u; for (auto &it : m_queue) lambda(it, i++); }
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

template <typename QueueType> class DoubleQueue : public MultiQueue<QueueType, 2u> { public: using MultiQueue<QueueType, 2u>::MultiQueue; };
class DoubleObjectQueue : public DoubleQueue<ObjectQueue> { public: using DoubleQueue<ObjectQueue>::DoubleQueue; };
class DoubleProjectileQueue : public DoubleQueue<ProjectileQueue> { public: using DoubleQueue<ProjectileQueue>::DoubleQueue; };
class DoubleActorQueue : public DoubleQueue<MainActorQueue> { public: using DoubleQueue<MainActorQueue>::DoubleQueue; };