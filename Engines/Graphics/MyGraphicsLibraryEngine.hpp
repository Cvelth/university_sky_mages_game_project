#pragma once
#include <memory>
#include "MGL_AdditionalStructures.hpp"
class MGLWindow;
class Camera;
class ControllerInterface;
class MainActorQueue;
class DoubleProjectileQueue;
class ObjectQueue;
class IndependentObject;
class HUD_RenderInfo;

class MyGraphicsLibraryEngine {
private:
	MGLWindow *m_window;
	std::shared_ptr<Camera> m_camera;
	DoubleProjectileQueue &m_projectile_queue;
	ObjectQueue &m_miscellaneous_queue;
	MainActorQueue &m_actor_queue;
	HUD_RenderInfo *m_hud;

	MapProgram m_map_program;
	QueueProgram m_projectile_queue_program,
		m_miscellaneous_queue_program, 
		m_actor_queue_program;
	HUDProgram m_HUD_program;
public:
	MyGraphicsLibraryEngine(const char* title, size_t width, size_t height, bool isFullscreen, MainActorQueue &mq, DoubleProjectileQueue &pq, ObjectQueue &oq);
	~MyGraphicsLibraryEngine();

	virtual bool isWindowClosed();

	virtual size_t width() const;
	virtual size_t height() const;

	virtual void clearWindow();
	virtual void update();
	virtual void pollEvents();
	void waitEvents();

	virtual void insertController(ControllerInterface* controller);
	virtual void insertHUD(HUD_RenderInfo *hud);

	virtual void recalculateCamera();
	virtual void recalculateProjection();
	virtual void recalculateInstancing();

	virtual void initializeMapRendering(std::shared_ptr<Camera> camera);
	virtual void renderMap();
	virtual void cleanMapRendering();

	virtual void initializeQueueRendering();
	virtual void renderQueues();
	virtual void cleanQueueRendering();

	virtual void initializeHUDRendering();
	virtual void renderHUD();
	virtual void cleanHUDRendering();
};