#pragma once
class GameController;
class GameCamera;

class GameControllerInterface {
	GameController* m_controller;
public:
	GameControllerInterface();
	~GameControllerInterface();

	inline GameController* controller() {
		return m_controller;
	}
	inline GameController* operator*() {
		return controller();
	}

	void startCameraControl(GameCamera* camera);
	void stopCameraControl();
};

