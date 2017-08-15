#pragma once
#include <set>
#include <functional>

class IndependentObject;
class ObjectQueue {
private:
	std::set<IndependentObject*> m_queue;
public:
	ObjectQueue();
	~ObjectQueue();

	void add(IndependentObject* object);
	void remove(IndependentObject* object);

	void for_each(const std::function<void(IndependentObject*)>& lambda);
};