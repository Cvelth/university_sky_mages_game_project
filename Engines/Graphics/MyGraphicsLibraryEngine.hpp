#pragma once
#include "MGL_AdditionalStructures.hpp"

class MGLWindow;
class Camera;
class GameControllerInterface;
class ObjectQueue;
class IndependentObject;
class HUD_RenderInfo;

class MyGraphicsLibraryEngine {
private:
	MGLWindow *m_window;
	Camera *m_camera;
	ObjectQueue *m_queue;
	HUD_RenderInfo *m_hud;

	MapProgram m_map_program;
	QueueProgram m_queue_program;
	HUDProgram m_HUD_program;
public:
	MyGraphicsLibraryEngine();
	~MyGraphicsLibraryEngine();

	virtual void initialize();
	virtual void clean();

	virtual void initializeWindow(const char* title, size_t width, size_t height, bool isFullscreen);
	virtual void destroyWindow();
	virtual bool isWindowClosed();

	virtual size_t width() const;
	virtual size_t height() const;

	virtual void clearWindow();
	virtual void update();
	virtual void pollEvents();
	void waitEvents();

	virtual void insertController(GameControllerInterface* controller);
	virtual void initializeQueue(ObjectQueue *queue = nullptr);
	virtual void addToRenderQueue(IndependentObject* go);
	virtual void insertHUD(HUD_RenderInfo *hud);

	virtual void recalculateCamera();
	virtual void recalculateProjection();
	virtual void recalculateInstancing();

	virtual void initializeMapRendering(Camera* camera);
	virtual void renderMap();
	virtual void cleanMapRendering();

	virtual void initializeQueueRendering();
	virtual void renderQueue();
	virtual void cleanQueueRendering();

	virtual void initializeHUDRendering();
	virtual void renderHUD();
	virtual void cleanHUDRendering();
};