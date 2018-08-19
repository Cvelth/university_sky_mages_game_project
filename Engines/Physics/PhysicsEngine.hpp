#pragma once
#include <memory>
#include <functional>
#include "shared/vector.hpp"
class ObjectQueue;
class MainActorQueue;
class DoubleProjectileQueue;
class IndependentObject;
class IndependentObjectState;
class MainActor;
class Map;

class PhysicsEngine {
private:
	static size_t UpdateInterval;
	MainActorQueue *m_actor_queue;
	DoubleProjectileQueue *m_projectile_queue;
	ObjectQueue *m_object_queue;
	std::shared_ptr<Map> m_map;

	bool m_is_initialized;
protected:
	std::function<bool(void)> m_finish_flag_access;

	static vector const calculateGravityForce();
	static scalar const calculateDimentionalDragForce(scalar const& speed, scalar const& area);
	static vector const calculateDragForce(vector const& speed, vector const& size);

	static void processForces(std::shared_ptr<IndependentObjectState> os);
	static void processMovement(std::shared_ptr<IndependentObjectState> os, std::shared_ptr<Map> map);
	static void processWeaponry(std::shared_ptr<MainActor> ma, DoubleProjectileQueue *projectile_queue);
public:
	PhysicsEngine();
	PhysicsEngine(std::function<bool(void)> const& finishFlagAccess,
		MainActorQueue *actor_queue = nullptr, DoubleProjectileQueue *projectile_queue = nullptr, ObjectQueue *object_queue = nullptr);
	~PhysicsEngine();

	static size_t getUpdateInterval();
	static void changeUpdateInterval(size_t microseconds);

	void initialize(std::function<bool(void)> const &finishFlagAccess, 
		MainActorQueue *actor_queue, DoubleProjectileQueue *projectile_queue, ObjectQueue *object_queue);
	void initializeCollisionSystem(std::shared_ptr<Map> map);
	void addObject(std::shared_ptr<IndependentObject> object);
	void removeObject(std::shared_ptr<IndependentObject> object);
	void addActor(std::shared_ptr<MainActor> actor);
	void removeActor(std::shared_ptr<MainActor> actor);
	void loop(bool destroy_engine_after_exit = false);
	void clean();
};