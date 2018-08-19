#pragma once
#include <memory>
class MyGraphicsLibraryEngine;
class ControllerInterface;
class Camera;
class IndependentObject;
class MainActorQueue;
class DoubleProjectileQueue;
class ObjectQueue;
class HUD_RenderInfo;

//Class to store game window.
//It stores and handles connection between the window handler(GLFWwindow struct) and chosen Engine.
class Window {
private:
	MyGraphicsLibraryEngine *m_graphics;
	ControllerInterface* m_controller;
	std::shared_ptr<Camera> m_camera;
	size_t m_update_interval;

public:
	//Constructs new window and engine using initialize function (protected).
	//Needed to pass window title and dimention sizes.
	Window(char const *title, size_t width, size_t height, bool isFullscreen,
		   MainActorQueue &main_actor_queue, DoubleProjectileQueue &projectile_queue, ObjectQueue &object_queue);

	//Initialiazes window event handling. Redirects all future events to controller.
	void changeController(ControllerInterface *controller, bool deleteOldOne = true);

	//Adds camera in the window to be shown using GameCamera.
	void insertCamera(std::shared_ptr<Camera> camera);

	//Changes loop's run interval
	inline void changeUpdateInterval(size_t microseconds) {	m_update_interval = microseconds; }
	//Returns loop's run interval
	inline size_t getUpdateInterval() const { return m_update_interval;	}

	//Returns aspect ration of the window in the state it is now.
	float currentAspectRatio() const;
	//Returns true if window is closed or would be closed soon.
	bool isWindowClosed();

	//Initializes HUD rendering.
	void insertHUDRenderInfo(HUD_RenderInfo *hud);

	//Desctructs class and all dependent on it. Uses clean function.
	~Window();

	//Main game loop. 
	//Initializes engine rendering state (sends data to graphical device).
	//In loop uses the redrawing in order to update the view and polls new events in order for engines to handle them.
	//After window is manually closed by the user, it cleans rendering data using engine and finishes executing.
	void loop(bool destroy_window_after_exit);
};