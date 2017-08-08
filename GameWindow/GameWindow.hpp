#pragma once
class MyGraphicsLibraryEngine;
class GameControllerInterface;
class GameCamera;
class GameMap;
class AbstractGameObject;
class HUD_RenderInfo;

//Class to store game window.
//It stores and handles connection between the window handler(GLFWwindow struct) and chosen Engine.
class GameWindow {
private:
	MyGraphicsLibraryEngine* m_graphics;
	GameControllerInterface* m_controller;
	GameCamera* m_camera;
	bool isMapInserted;
	bool isControllerInserted;
	size_t m_update_interval;

protected:
	//Creates engine instance, shows a window and initializes the connection between them.
	void initialize();

	//Cleans all the instances of windows and engines created during work process.
	void clean();

	//static void errorCallback(int errorCode, const char* description);
public:

	//Constructs new window and engine using initialize function (protected).
	//Needed to pass window title and dimention sizes.
	GameWindow(char const *title, size_t width, size_t height, bool isFullscreen);

	//Initialiazes window event handling. Redirects all future events to controller.
	void insertController(GameControllerInterface *controller);

	//Changes controller for events to be redirected to.
	void changeController(GameControllerInterface *controller, bool deleteOldOne = true);

	//Adds map in the window to be shown using GameCamera.
	void insertMap(GameMap *map);

	//Changes loop's run interval
	void changeUpdateInterval(size_t microseconds);
	//Returns loop's run interval
	size_t getUpdateInterval();

	//Returns true if window is closed or would be closed soon.
	bool isWindowClosed();

	//Adds an object in the queue to be rendered.
	void addToRenderQueue(AbstractGameObject *go);

	//Initializes HUD rendering.
	void insertHUDRenderInfo(HUD_RenderInfo *hud);

	//Desctructs class and all dependent on it. Uses clean function.
	~GameWindow();

	//Main game loop. 
	//Initializes engine rendering state (sends data to graphical device).
	//In loop uses the redrawing in order to update the view and polls new events in order for engines to handle them.
	//After window is manually closed by the user, it cleans rendering data using engine and finishes executing.
	void loop(bool destroy_window_after_exit);
};

