#pragma once
struct GLFWwindow;
class AbstractGraphicsEngine;
class GameCamera;
class GameMap;

class GameWindow {
private:
	GLFWwindow* m_window;
	AbstractGraphicsEngine* m_graphics;
	GameCamera* m_camera;

	size_t m_width;
	size_t m_height;
protected:
	void initialize();
	void clean();

	static void errorCallback(int errorCode, const char* description);
public:
	GameWindow(char* title, size_t width, size_t height, GameMap* map, bool insertDefaultCallback = true);
	~GameWindow();
	int loop();
};

