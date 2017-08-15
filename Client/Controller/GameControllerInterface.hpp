#pragma once
class GameController;
class Camera;
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

	void startCameraControl(Camera* camera);
	void stopCameraControl();

	void setMainActor(MainActor* actor);
	void removeMainActor();

	void startKeyControl(KeyLayout const *keys);
	void stopKeyControl();
};

