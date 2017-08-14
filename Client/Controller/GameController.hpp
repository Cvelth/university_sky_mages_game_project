#pragma once
#include "MGL\Events\EmptyEventHandler.hpp"

class GameCamera;
class MainActor;
struct KeyLayout;

class GameController : public mgl::EmptyEventHandler {
protected:
	GameCamera* m_controlledCamera;
	MainActor* m_actor;
	KeyLayout const *m_keys;

	double m_x;
	double m_y;

	int current_screen_width;
	int current_screen_height;
public:
	GameController();
	~GameController();

	virtual void mouseButtonEvent(GLFWwindow* w, mgl::MouseButton button,
								  mgl::MouseAction action, mgl::Mods mods) override;
	virtual void mouseMoveEvent(GLFWwindow* w, double x, double y) override;
	virtual void mouseScrollEvent(GLFWwindow* w, double x, double y) override;
	virtual void resizeEvent(GLFWwindow* w, int x, int y) override;
	virtual void keyEvent(GLFWwindow* w, mgl::Key key, int scancode,
						  mgl::KeyAction action, mgl::Mods mods) override;

	void startCameraControl(GameCamera* camera);
	void stopCameraControl();

	void setMainActor(MainActor* actor);
	void removeMainActor();

	void startKeyControl(KeyLayout const *keys);
	void stopKeyControl();
};

