#pragma once
#include <functional>
#include "vector.hpp"
class PhysicalObjectsQueue;
class AbstractGameObject;
class GameMap;
class IndependentObjectState;

class PhysicsEngine {
private:
	static size_t UpdateInterval;
	PhysicalObjectsQueue* m_queue;
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
	PhysicsEngine(std::function<bool(void)> const& finishFlagAccess);
	~PhysicsEngine();

	static size_t getUpdateInterval();
	static void changeUpdateInterval(size_t microseconds);

	void initialize(std::function<bool(void)> const &finishFlagAccess);
	void initializeCollisionSystem(GameMap *map);
	void addObject(AbstractGameObject* object);
	void removeObject(AbstractGameObject* object);
	void loop(bool destroy_engine_after_exit = false);
	void clean();
};