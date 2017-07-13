#pragma once
struct GLFWwindow;
class AbstractGraphicsEngine;
class GameCamera;
class GameMap;

class GameWindow {
private:
	GLFWwindow* window;
	AbstractGraphicsEngine* graphics;
	GameCamera* camera;

	size_t width;
	size_t height;
protected:
	void initialize();
	void clean();

	static void errorCallback(int errorCode, const char* description);
public:
	GameWindow(char* title, size_t width, size_t height, GameMap* map, bool insertDefaultCallback = true);
	~GameWindow();
	int loop();
};

