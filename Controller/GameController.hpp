#pragma once
#include "Events\EmptyEventHandler.hpp"

class GameCamera;
class ControllableActor;
struct KeyLayout;

class GameController : public mgl::EmptyEventHandler {
protected:
	GameCamera* m_controlledCamera;
	ControllableActor* m_actor;
	KeyLayout const *m_keys;

	int current_screen_width;
public:
	GameController();
	~GameController();

	virtual void mouseScrollEvent(GLFWwindow* w, double x, double y) override;
	virtual void resizeEvent(GLFWwindow* w, int x, int y) override;
	virtual void keyEvent(GLFWwindow* w, mgl::Key key, int scancode,
						  mgl::KeyAction action, mgl::Mods mods) override;

	void startCameraControl(GameCamera* camera);
	void stopCameraControl();

	void setMainActor(ControllableActor* actor);
	void removeMainActor();

	void startKeyControl(KeyLayout const *keys);
	void stopKeyControl();
};

