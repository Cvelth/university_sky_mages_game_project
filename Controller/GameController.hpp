#pragma once
#include "Events\EmptyEventHandler.hpp"

class GameCamera;

class GameController : public mgl::EmptyEventHandler {
protected:
	GameCamera* m_controlledCamera;
public:
	GameController();
	~GameController();

	virtual void mouseScrollEvent(GLFWwindow* w, double x, double y) override;

	void startCameraControl(GameCamera* camera);
	void stopCameraControl();
};

