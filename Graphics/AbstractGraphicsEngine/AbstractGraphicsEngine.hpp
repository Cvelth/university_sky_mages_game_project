#pragma once
struct GLFWwindow;

//Abtract engine. Used for defining of the base structure of all the other engines,
//being inherited by them. Stores window-data(pointer to them).
class AbstractGraphicsEngine {
protected:
	GLFWwindow* m_window;
	size_t m_window_width;
	size_t m_window_height;
protected:
	//Inner initialization of window, specific to engine.
	virtual void initializeWindow(char* title, size_t width, size_t height, bool isFullscreen) abstract;
public:
	//Empty constructor. Initialize needs to be called manually.
	AbstractGraphicsEngine();
	//Empty destructor. Clean needs to be called manually.
	~AbstractGraphicsEngine();

	//Initializes engine.
	virtual void initialize() abstract;
	//Destroys engine and cleans data after it.
	virtual void clean() abstract;

	//Outter initilization of window. Uses initializeWindow.
	void createWindow(char* title, size_t width, size_t height, bool isFullscreen = false);
	//Window destruction and cleaning handling.
	virtual void destroyWindow() abstract;

	//Creates rendering system and sends data to GPU device.
	virtual void initializeRenderProcess() abstract;
	//Updates screen, using data send with initializeRenderProcess.
	virtual void renderProcess() abstract;
	//Cleans data after the rendering is no longer needed.
	virtual void clearRenderProcess() abstract;

	//Returns true if window was manually closed by user and expects to be cleaned. False otherwise.
	virtual bool isWindowClosed() abstract;

	//Returns window handle.
	inline GLFWwindow* window() { return m_window; }
	//Returns width of the window as an unsigned int
	inline size_t width() {	return m_window_width; }
	//Returns height of the window as an unsigned int
	inline size_t height() { return m_window_height; }
};

