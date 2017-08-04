#pragma once
struct GLFWwindow;
class GameControllerInterface;
class GameMap;
class GameCamera;
class RenderQueue;

//Abtract engine. Used for defining of the base structure of all the other engines,
//being inherited by them. Stores window-data(pointer to them).
class AbstractGraphicsEngine {
protected:
	GLFWwindow* m_window;
	size_t m_window_width;
	size_t m_window_height;

	GameCamera *m_camera;
	RenderQueue *m_queue;
protected:
	//Inner initialization of window, specific to engine.
	virtual void initializeWindow(const char* title, size_t width, size_t height, bool isFullscreen) abstract;
public:
	//Empty constructor. Initialize needs to be called manually.
	AbstractGraphicsEngine();
	//Virtual destructor.
	virtual ~AbstractGraphicsEngine();

	//Initializes engine.
	virtual void initialize() abstract;
	//Destroys engine and cleans data after it.
	virtual void clean() abstract;

	//Outter initilization of window. Uses initializeWindow.
	void createWindow(const char* title, size_t width, size_t height, bool isFullscreen = false);
	//Window destruction and cleaning handling.
	virtual void destroyWindow() abstract;

	//Initialization of Queue.
	virtual void initializeQueue();

	//Updates screen accordingly to last rendered data.
	virtual void update() abstract;

	//Processes events which have happened since last call of this function.
	virtual void pollEvents() abstract;

	//Initializes event handling. All future events will be redirected to *controler*.
	virtual void insertController(GameControllerInterface* controller) abstract;

	//Puts all the objects/textures/rendering data for all the map blocks into graphical memory.
	//Gets ready for map rendering.
	virtual void initializeMapRendering(GameCamera* camera) abstract;
	//Shows part of the map which are in the frame of current camera position.
	virtual void renderMap() abstract;
	//Clears data after map and all its blocks.
	virtual void cleanMapRendering() abstract;

	//Puts all the objects/textures/rendering data for all the queue elements into graphical memory.
	//Gets ready for queue rendering.
	virtual void initializeQueueRendering() abstract;
	//Shows elements of the queue which are in the frame of current camera position.
	virtual void renderQueue() abstract;
	//Clears data after queue and all its elements.
	virtual void cleanQueueRendering() abstract;
	
	//Returns true if window was manually closed by user and expects to be cleaned. False otherwise.
	virtual bool isWindowClosed() abstract;

	//Returns RenderQueue handle;
	inline RenderQueue* getRenderQueue() { return m_queue; }
	//Returns window handle.
	inline GLFWwindow* window() { return m_window; }
	//Returns width of the window as an unsigned int
	inline size_t width() {	return m_window_width; }
	//Returns height of the window as an unsigned int
	inline size_t height() { return m_window_height; }
};

