#pragma once
#include <cstdint>

struct GLFWwindow;
typedef struct VkInstance_T* VkInstance;
typedef struct VkPhysicalDevice_T* VkPhysicalDevice;
typedef struct VkDevice_T* VkDevice;
typedef struct VkQueue_T* VkQueue;
typedef uint64_t VkSurfaceKHR;
class GameCamera;
class GameMap;

class GameWindow {
private:
	GLFWwindow* window;
	GameCamera* camera;
	VkInstance* instance;
	VkPhysicalDevice* physicalDevice;
	VkDevice* device;
	VkQueue* queue;
	VkSurfaceKHR* surface;

	size_t width;
	size_t height;

	bool validationEnabled;
protected:
	void initializeGLFW();
	void initializeWindow(const char* title);
	void initializeGraphics();

	void cleanGLFW();
	void cleanWindow();
	void cleanGraphics();

	virtual void initializeRenderProcess();
	virtual void renderProcess();
	virtual void clearRenderProcess();

	static void errorCallback(int errorCode, const char* description);
public:
	GameWindow(char* title, size_t width, size_t height, GameMap* map, bool insertDefaultCallback = true);
	~GameWindow();
	int loop();
};

