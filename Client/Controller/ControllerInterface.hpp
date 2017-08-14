#pragma once
class Controller;
class Camera;
class MainActor;
struct KeyLayout;

class ControllerInterface {
	Controller* m_controller;
public:
	ControllerInterface();
	~ControllerInterface();

	inline Controller* controller() {
		return m_controller;
	}
	inline Controller* operator*() {
		return controller();
	}

	void startCameraControl(Camera* camera);
	void stopCameraControl();

	void setMainActor(MainActor* actor);
	void removeMainActor();

	void startKeyControl(KeyLayout const *keys);
	void stopKeyControl();
};

