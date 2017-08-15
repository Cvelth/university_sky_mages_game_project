#pragma once
#include <functional>
#include "shared\vector.hpp"
class ObjectQueue;
class IndependentObject;
class IndependentObjectState;
class GameMap;

class PhysicsEngine {
private:
	static size_t UpdateInterval;
	ObjectQueue* m_queue;
	GameMap* m_map;

	bool m_is_initialized;
protected:
	std::function<bool(void)> m_finish_flag_access;

	static vector const calculateGravityForce();
	static scalar const calculateDimentionalDragForce(scalar const& speed, scalar const& area);
	static vector const calculateDragForce(vector const& speed, vector const& size);

	static void processForces(IndependentObjectState *os);
	static void processMovement(IndependentObjectState *os, GameMap *map);
public:
	PhysicsEngine();
	PhysicsEngine(std::function<bool(void)> const& finishFlagAccess, ObjectQueue* queue = nullptr);
	~PhysicsEngine();

	static size_t getUpdateInterval();
	static void changeUpdateInterval(size_t microseconds);

	void initialize(std::function<bool(void)> const &finishFlagAccess, ObjectQueue* queue);
	void initializeCollisionSystem(GameMap *map);
	void addObject(IndependentObject *object);
	void removeObject(IndependentObject *object);
	void loop(bool destroy_engine_after_exit = false);
	void clean();
};