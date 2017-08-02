#pragma once
#include <functional>
class PhysicalObjectsQueue;
class AbstractGameObject;

class PhysicsEngine {
private:
	static size_t UpdateInterval;
	PhysicalObjectsQueue* m_queue;

	bool m_is_initialized;
protected:
	std::function<bool(void)> m_finish_flag_access;

	static void processGravity(AbstractGameObject* go);
	static void processAcceleration(AbstractGameObject* go);
	static void processMovement(AbstractGameObject* go);
	void processQueue();
public:
	PhysicsEngine();
	PhysicsEngine(std::function<bool(void)> const& finishFlagAccess);
	~PhysicsEngine();

	static size_t getUpdateInterval();
	static void changeUpdateInterval(size_t microseconds);

	void initialize(std::function<bool(void)> const& finishFlagAccess);
	void addObject(AbstractGameObject* object);
	void removeObject(AbstractGameObject* object);
	void loop(bool destroy_engine_after_exit = false);
	void clean();
};