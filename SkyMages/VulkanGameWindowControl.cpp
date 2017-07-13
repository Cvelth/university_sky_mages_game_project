#include "GameWindow.hpp"
#include "vulkan.h"
#include "glfw3.h"

void GameWindow::initializeRenderProcess() {
	//auto error = glfwCreateWindowSurface()
	//glClearColor(0.1, 0, 0, 1);
	glfwSwapInterval(1);
}

void GameWindow::renderProcess() {
	//glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void GameWindow::clearRenderProcess() {

}
