#include "GameWindow.hpp"
#include "GLHeaders.hpp"

void GameWindow::initializeGL() {
	glClearColor(0.1, 0, 0, 1);
	glfwSwapInterval(1);
}

void GameWindow::updateGL() {
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void GameWindow::clearGL() {

}
