#pragma once
class GameController;
class GameCamera;
class MainActor;
struct KeyLayout;

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

	void setMainActor(MainActor* actor);
	void removeMainActor();

	void startKeyControl(KeyLayout const *keys);
	void stopKeyControl();
};

