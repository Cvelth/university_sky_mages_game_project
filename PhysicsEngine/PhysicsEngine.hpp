#pragma once
#include <functional>
class PhysicalObjectsQueue;
class AbstractGameObject;

class PhysicsEngine {
private:
	size_t m_update_interval;
	PhysicalObjectsQueue* m_queue;

	bool m_is_initialized;
protected:
	std::function<bool(void)> m_finish_flag_access;

	void processQueue();
public:
	PhysicsEngine();
	PhysicsEngine(std::function<bool(void)> const& finishFlagAccess);
	~PhysicsEngine();

	size_t getUpdateInterval();
	void changeUpdateInterval(size_t microseconds);

	void initialize(std::function<bool(void)> const& finishFlagAccess);
	void addObject(AbstractGameObject* object);
	void removeObject(AbstractGameObject* object);
	void loop(bool destroy_engine_after_exit = false);
	void clean();
};