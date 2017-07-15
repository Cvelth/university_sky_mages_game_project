#pragma once
struct GLFWwindow;

class AbstractGraphicsEngine {
protected:
	GLFWwindow* window;
	size_t window_width;
	size_t window_height;
protected:
	virtual void setWindowSettings() abstract;
public:
	virtual void initialize() abstract;
	virtual void clean() abstract;

	GLFWwindow* createWindow(char* title, size_t width, size_t height);
	void destroyWindow();

	virtual void initializeRenderProcess() abstract;
	virtual void renderProcess() abstract;
	virtual void clearRenderProcess() abstract;
public:
	AbstractGraphicsEngine();
	~AbstractGraphicsEngine();
};

