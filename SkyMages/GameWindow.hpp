#pragma once
#include "AbstractException.hpp"

struct GLFWwindow;
class GameCamera;
class GameMap;

class GLFWInitializationException : AbstractException {};
class WindowInitializationException  : AbstractException {};
class GLFWCallBackException : AbstractException {};

class GameWindow {
private:
	GLFWwindow* window;
	GameCamera* camera;
protected:
	void initializeGLFW();
	void initializeWindow();

	virtual void initializeGL();
	virtual void updateGL();
	virtual void clearGL();

	static void errorCallback(int errorCode, const char* description);
public:
	GameWindow(char* name, size_t width, size_t height, GameMap* map, bool insertDefaultCallback = true);
	~GameWindow();
	int loop();
	void makeCurrent();
};

