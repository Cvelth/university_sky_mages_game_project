#pragma once
#include "Events\EmptyEventHandler.hpp"

class GameCamera;
class ControllableActor;

class GameController : public mgl::EmptyEventHandler {
protected:
	GameCamera* m_controlledCamera;
	ControllableActor* m_actor;

	int current_screen_width;
public:
	GameController();
	~GameController();

	virtual void mouseScrollEvent(GLFWwindow* w, double x, double y) override;
	virtual void resizeEvent(GLFWwindow* w, int x, int y) override;

	void startCameraControl(GameCamera* camera);
	void stopCameraControl();

	void setMainActor(ControllableActor* actor);
	void removeMainActor();
};

