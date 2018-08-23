#pragma once
#include <memory>
#include <functional>
#include "shared/vector.hpp"
class ObjectQueue;
class MainActorQueue;
class DoubleProjectileQueue;
class IndependentObjectState;
class MainActor;
class Map;
class ShootableObject;

class PhysicsEngine {
private:
	static size_t UpdateInterval;
	MainActorQueue &m_actor_queue;
	DoubleProjectileQueue &m_projectile_queue;
	ObjectQueue &m_object_queue;
	std::shared_ptr<Map> m_map;
protected:
	std::function<bool(void)> m_finish_flag_access;

	static vector const calculateGravityForce();
	static scalar const calculateDimentionalDragForce(scalar const& speed, scalar const& area);
	static vector const calculateDragForce(vector const& speed, vector const& size);

	static void processForces(std::shared_ptr<IndependentObjectState> os);
	static bool processMovement(std::shared_ptr<IndependentObjectState> os, std::shared_ptr<Map> map);
	static void processWeaponry(std::shared_ptr<MainActor> ma, DoubleProjectileQueue &projectile_queue);
	static bool processTargeting(std::shared_ptr<ShootableObject> so, MainActorQueue &actors);
public:
	PhysicsEngine(std::function<bool(void)> const& finishFlagAccess,
		MainActorQueue &actor_queue, DoubleProjectileQueue &projectile_queue, ObjectQueue &object_queue);
	~PhysicsEngine() {}

	static size_t getUpdateInterval();
	static void changeUpdateInterval(size_t microseconds);
	
	void initializeCollisionSystem(std::shared_ptr<Map> map);
	void loop(bool destroy_engine_after_exit = false);
};