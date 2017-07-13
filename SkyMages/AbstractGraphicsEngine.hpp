#pragma once
struct GLFWwindow;

class AbstractGraphicsEngine {
protected:
	GLFWwindow* window;
protected:
	virtual void setWindowSettings() abstract;
public:
	virtual void initialize() abstract;
	virtual void clean() abstract;

	GLFWwindow* createWindow(char* title, size_t width, size_t height);
	virtual void destroyWindow() abstract;

	virtual void initializeRenderProcess() abstract;
	virtual void renderProcess() abstract;
	virtual void clearRenderProcess() abstract;
public:
	AbstractGraphicsEngine();
	~AbstractGraphicsEngine();
};

