#pragma once
#include "Events\EmptyEventHandler.hpp"

class GameCamera;
class ControllableActor;

class GameController : public mgl::EmptyEventHandler {
protected:
	GameCamera* m_controlledCamera;
	ControllableActor* m_actor;
public:
	GameController();
	~GameController();

	virtual void mouseScrollEvent(GLFWwindow* w, double x, double y) override;

	void startCameraControl(GameCamera* camera);
	void stopCameraControl();

	void setMainActor(ControllableActor* actor);
	void removeMainActor();
};

